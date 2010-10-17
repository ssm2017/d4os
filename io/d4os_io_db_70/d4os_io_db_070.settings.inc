<?php
/**
 * @package    d4os_io_db
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * Globals
 */
global $db_url, $d4os_db_prefix, $d4os_same_db;

/**
 * Build a list of needed databases
 */
function d4os_io_db_070_build_bases_list() {
  $bases = array(
    'none'            => t('None'),
    'default'         => t('Website'),
    'os_robust'       => t('Robust'),
  );
  if (module_exists('d4os_io_services_profile')) {
    $bases['os_profile'] = t('Profile');
  }
  if (module_exists('d4os_io_services_offline_message')) {
    $bases['os_offline_message'] = t('offline_message');
  }
  if (module_exists('d4os_io_services_search')) {
    $bases['os_search'] = t('Search');
  }
  return $bases;
}

if (!is_array($db_url)) {
  $db_url = array(
    'default' => $db_url
  );
}
$d4os_same_db = array('none'=>'none');

// single
$db_url['os_single'] = 'mysqli://'
. variable_get('d4os_io_db_070_single_username', 'root')
.':'
. variable_get('d4os_io_db_070_single_password', 'root')
. '@'
. variable_get('d4os_io_db_070_single_host', 'localhost')
. '/'
. variable_get('d4os_io_db_070_single_db', 'opensim');
$d4os_db_prefix['os_single'] = variable_get('d4os_io_db_070_single_prefix', '');

// robust
$db_url['os_robust'] = 'mysqli://'
. variable_get('d4os_io_db_070_multiple_robust_username', 'root')
.':'
. variable_get('d4os_io_db_070_multiple_robust_password', 'root')
. '@'
. variable_get('d4os_io_db_070_multiple_robust_host', 'localhost')
. '/'
. variable_get('d4os_io_db_070_multiple_robust_db', 'robust');
$d4os_db_prefix['os_robust'] = variable_get('d4os_io_db_070_multiple_robust_prefix', '');
$d4os_same_db['os_robust'] = variable_get('d4os_io_db_070_multiple_robust_same', 'none');

// profile
$db_url['os_profile'] = 'mysqli://'
. variable_get('d4os_io_db_070_multiple_profile_username', 'root')
.':'
. variable_get('d4os_io_db_070_multiple_profile_password', 'root')
. '@'
. variable_get('d4os_io_db_070_multiple_profile_host', 'localhost')
. '/'
. variable_get('d4os_io_db_070_multiple_profile_db', 'profile');
$d4os_db_prefix['os_profile'] = variable_get('d4os_io_db_070_multiple_profile_prefix', '');
$d4os_same_db['os_profile'] = variable_get('d4os_io_db_070_multiple_profile_same', 'none');

// os_offline_message
$db_url['os_offline_message'] = 'mysqli://'
. variable_get('d4os_io_db_070_multiple_offline_message_username', 'root')
.':'
. variable_get('d4os_io_db_070_multiple_offline_message_password', 'root')
. '@'
. variable_get('d4os_io_db_070_multiple_offline_message_host', 'localhost')
. '/'
. variable_get('d4os_io_db_070_multiple_offline_message_db', 'profile');
$d4os_db_prefix['os_offline_message'] = variable_get('d4os_io_db_070_multiple_offline_message_prefix', '');
$d4os_same_db['os_offline_message'] = variable_get('d4os_io_db_070_multiple_offline_message_same', 'none');

// search
$db_url['os_search'] = 'mysqli://'
. variable_get('d4os_io_db_070_multiple_search_username', 'root')
.':'
. variable_get('d4os_io_db_070_multiple_search_password', 'root')
. '@'
. variable_get('d4os_io_db_070_multiple_search_host', 'localhost')
. '/'
. variable_get('d4os_io_db_070_multiple_search_db', 'search');
$d4os_db_prefix['os_search'] = variable_get('d4os_io_db_070_multiple_search_prefix', '');
$d4os_same_db['os_search'] = variable_get('d4os_io_db_070_multiple_search_same', 'none');
