<?php
/**
 * @package    d4os_io db 070
 * @copyright Copyright (C) 2010-2012 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @link      http://www.d4os.org
 * @license   GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

module_load_include('settings.inc', 'd4os', 'io/connectors/db_070/db_070');

/**
 * Set the active database
 * @param String The database config name
 */
function d4os_io_db_070_set_active($name = 'default') {
  global $d4os_same_db;

  // set db for default value
  if ($name == 'default') {
    db_set_active('default');
    return;
  }
  // set db for multiple values
  switch (variable_get('d4os_io_db_070_type', 0)) {
    // same as website
    case 0:
      return;
      break;
    // single
    case 1:
      db_set_active('os_single');
      break;
    // multiple
    case 2:
      $name = $d4os_same_db[$name] != 'none' ? $d4os_same_db[$name] : $name;
      db_set_active($name);
      break;
  }
}

/**
 * Check if a datastore is alive
 * @param String The database config name
 * @param String The user readable datastore name
 * @return Bool True of false
 */
function d4os_io_db_070_mysql_is_alive($name, $datastore) {
  global $d4os_same_db, $databases;

  switch (variable_get('d4os_io_db_070_type', 0)) {
    // same as website
    case 0:
      return TRUE;
      break;
    // single
    case 1:
      $name = 'os_single';
      break;
    // multiple
    case 2:
      $name = $d4os_same_db[$name] != 'none' ? $d4os_same_db[$name] : $name;
      break;
  }

  try {
    $db_string = 'mysql:host='. $databases[$name]['default']['host']. ';dbname='. $databases[$name]['default']['database'];
    $dbh = new PDO($db_string, $databases[$name]['default']['username'], $databases[$name]['default']['password']);
    $dbh = null;
    return TRUE;
  }
  catch (PDOException $e) {
    drupal_set_message(t('The !datastore datastore is not available.', array('!datastore' => $datastore)), 'error');
    drupal_set_message(t('Please inform the website admins.'), 'error');
    watchdog('d4os io db', 'The !datastore datastore is not available.', array('!datastore' => $datastore), WATCHDOG_EMERGENCY);
    return FALSE;
  }
}
