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
  global $d4os_same_db, $d4os_db_prefix, $db_prefix;
  static $temp_db_prefix;

  // set db for default value
  if ($name == 'default') {
    $db_prefix = $temp_db_prefix;
    $temp_db_prefix = NULL;
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
      $temp_db_prefix = $db_prefix;
      $db_prefix = $d4os_db_prefix['os_single'];
      db_set_active('os_single');
      break;
    // multiple
    case 2:
      $temp_db_prefix = $db_prefix;
      $name = $d4os_same_db[$name] != 'none' ? $d4os_same_db[$name] : $name;
      $db_prefix = $d4os_db_prefix[$name];
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
  global $d4os_same_db, $db_url;
  $params = array();
  switch (variable_get('d4os_io_db_070_type', 0)) {
    // same as website
    case 0:
      return TRUE;
      break;
    // single
    case 1:
      $url = $db_url['os_single'];
      break;
    // multiple
    case 2:
      $name = $d4os_same_db[$name] != 'none' ? $d4os_same_db[$name] : $name;
      $url = $db_url[$name];
      break;
  }
  // Decode url-encoded information in the db connection string
  $url = parse_url($url);
  $url['user'] = urldecode($url['user']);
  // Test if database url has a password.
  $url['pass'] = isset($url['pass']) ? urldecode($url['pass']) : '';
  $url['host'] = urldecode($url['host']);
  $url['path'] = urldecode($url['path']);

  // Allow for non-standard MySQL port.
  if (isset($url['port'])) {
    $url['host'] = $url['host'] .':'. $url['port'];
  }

  // - TRUE makes mysql_connect() always open a new link, even if
  //   mysql_connect() was called before with the same parameters.
  //   This is important if you are using two databases on the same
  //   server.
  // - 2 means CLIENT_FOUND_ROWS: return the number of found
  //   (matched) rows, not the number of affected rows.
  $connection = @mysql_connect($url['host'], $url['user'], $url['pass'], TRUE, 2);
  if (!$connection || !mysql_select_db(substr($url['path'], 1))) {
    // Show error screen otherwise
    //_db_error_page(mysql_error());
    drupal_set_message(t('The !datastore datastore is not available.', array('!datastore' => $datastore)), 'error');
    drupal_set_message(t('Please inform the website admins.'), 'error');
    watchdog('d4os io db', 'The !datastore datastore is not available.', array('!datastore' => $datastore), WATCHDOG_EMERG);
    return FALSE;
  }
  mysql_close($connection);
  return TRUE;
}
