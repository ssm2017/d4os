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
/**
 * Globals
 */
global $d4os_same_db, $databases;

/**
 * Build a list of needed databases
 */
function d4os_io_db_070_build_bases_list() {
  $bases = array(
    'none' => t('None'),
    'default' => t('Website'),
    'os_robust' => t('Robust'),
  );
  if (module_exists('d4os_io_profiles')) {
    $bases['os_profile'] = t('Profiles');
  }
  if (module_exists('d4os_io_groups')) {
    $bases['os_groups'] = t('Groups');
  }
  if (module_exists('d4os_io_search')) {
    $bases['os_search'] = t('Search');
  }
  return $bases;
}

$d4os_same_db = array('none' => 'none');

// single
$databases['os_single']['default'] = array(
  'driver' => 'mysql',
  'host' => variable_get('d4os_io_db_070_single_host', 'localhost'),
  'database' => variable_get('d4os_io_db_070_single_db', 'opensim'),
  'username' => variable_get('d4os_io_db_070_single_username', 'root'),
  'password' => variable_get('d4os_io_db_070_single_password', 'root'),
  'prefix' => variable_get('d4os_io_db_070_single_prefix', '')
);
Database::addConnectionInfo('os_single', 'default', $databases['os_single']['default']);

// robust
$databases['os_robust']['default'] = array(
  'driver' => 'mysql',
  'host' => variable_get('d4os_io_db_070_multiple_robust_host', 'localhost'),
  'database' => variable_get('d4os_io_db_070_multiple_robust_db', 'opensim'),
  'username' => variable_get('d4os_io_db_070_multiple_robust_username', 'root'),
  'password' => variable_get('d4os_io_db_070_multiple_robust_password', 'root'),
  'prefix' => variable_get('d4os_io_db_070_multiple_robust_prefix', '')
);
Database::addConnectionInfo('os_robust', 'default', $databases['os_robust']['default']);
$d4os_same_db['os_robust'] = variable_get('d4os_io_db_070_multiple_robust_same', 'none');

// profile
if (module_exists('d4os_io_profiles')) {
  $databases['os_profile']['default'] = array(
    'driver' => 'mysql',
    'host' => variable_get('d4os_io_db_070_multiple_profile_host', 'localhost'),
    'database' => variable_get('d4os_io_db_070_multiple_profile_db', 'opensim'),
    'username' => variable_get('d4os_io_db_070_multiple_profile_username', 'root'),
    'password' => variable_get('d4os_io_db_070_multiple_profile_password', 'root'),
    'prefix' => variable_get('d4os_io_db_070_multiple_profile_prefix', '')
  );
  Database::addConnectionInfo('os_profile', 'default', $databases['os_profile']['default']);
  $d4os_same_db['os_profile'] = variable_get('d4os_io_db_070_multiple_profile_same', 'none');
}

// groups
if (module_exists('d4os_io_groups')) {
  $databases['os_groups']['default'] = array(
    'driver' => 'mysql',
    'host' => variable_get('d4os_io_db_070_multiple_groups_host', 'localhost'),
    'database' => variable_get('d4os_io_db_070_multiple_groups_db', 'opensim'),
    'username' => variable_get('d4os_io_db_070_multiple_groups_username', 'root'),
    'password' => variable_get('d4os_io_db_070_multiple_groups_password', 'root'),
    'prefix' => variable_get('d4os_io_db_070_multiple_groups_prefix', '')
  );
  Database::addConnectionInfo('os_groups', 'default', $databases['os_groups']['default']);
  $d4os_same_db['os_groups'] = variable_get('d4os_io_db_070_multiple_groups_same', 'none');
}

// search
if (module_exists('d4os_io_search')) {
  $databases['os_search']['default'] = array(
    'driver' => 'mysql',
    'host' => variable_get('d4os_io_db_070_multiple_search_host', 'localhost'),
    'database' => variable_get('d4os_io_db_070_multiple_search_db', 'opensim'),
    'username' => variable_get('d4os_io_db_070_multiple_search_username', 'root'),
    'password' => variable_get('d4os_io_db_070_multiple_search_password', 'root'),
    'prefix' => variable_get('d4os_io_db_070_multiple_search_prefix', '')
  );
  Database::addConnectionInfo('os_search', 'default', $databases['os_search']['default']);
  $d4os_same_db['os_search'] = variable_get('d4os_io_db_070_multiple_search_same', 'none');
}