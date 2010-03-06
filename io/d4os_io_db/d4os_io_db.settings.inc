<?php
/**
* @package    d4os
* @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
* @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
* D4os is free software. This version may have been modified pursuant
* to the GNU General Public License, and as distributed it includes or
* is derivative of works licensed under the GNU General Public License or
* other free or open source software licenses.
*/

global $db_url, $d4os_same_db;
$d4os_same_db = array('none'=>'none');
// single
$db_url['os_single'] = 'mysqli://'
. variable_get('d4os_io_db_single_username', 'root')
.':'
. variable_get('d4os_io_db_single_password', 'root')
. '@'
. variable_get('d4os_io_db_single_host', 'localhost')
. '/'
. variable_get('d4os_io_db_single_db', 'opensim');
$db_prefix['os_single'] = variable_get('d4os_io_db_single_prefix', '');

// asset
//$db_url['os_asset'] = 'mysqli://'
//. variable_get('d4os_io_db_multiple_asset_username', 'root')
//.':'
//. variable_get('d4os_io_db_multiple_asset_password', 'root')
//. '@'
//. variable_get('d4os_io_db_multiple_asset_host', 'localhost')
//. '/'
//. variable_get('d4os_io_db_multiple_asset_db', 'asset');
//$db_prefix['os_asset'] = variable_get('d4os_io_db_multiple_asset_prefix', '');
//$d4os_same_db['os_asset'] = variable_get('d4os_io_db_multiple_asset_same', 'none');

// auth
//$db_url['os_auth'] = 'mysqli://'
//. variable_get('d4os_io_db_multiple_auth_username', 'root')
//.':'
//. variable_get('d4os_io_db_multiple_auth_password', 'root')
//. '@'
//. variable_get('d4os_io_db_multiple_auth_host', 'localhost')
//. '/'
//. variable_get('d4os_io_db_multiple_auth_db', 'auth');
//$db_prefix['os_auth'] = variable_get('d4os_io_db_multiple_auth_prefix', '');
//$d4os_same_db['os_auth'] = variable_get('d4os_io_db_multiple_auth_same', 'none');

// estate
//$db_url['os_estate'] = 'mysqli://'
//. variable_get('d4os_io_db_multiple_estate_username', 'root')
//.':'
//. variable_get('d4os_io_db_multiple_estate_password', 'root')
//. '@'
//. variable_get('d4os_io_db_multiple_estate_host', 'localhost')
//. '/'
//. variable_get('d4os_io_db_multiple_estate_db', 'estate');
//$db_prefix['os_estate'] = variable_get('d4os_io_db_multiple_estate_prefix', '');
//$d4os_same_db['os_estate'] = variable_get('d4os_io_db_multiple_estate_same', 'none');

// grid
$db_url['os_grid'] = 'mysqli://'
. variable_get('d4os_io_db_multiple_grid_username', 'root')
.':'
. variable_get('d4os_io_db_multiple_grid_password', 'root')
. '@'
. variable_get('d4os_io_db_multiple_grid_host', 'localhost')
. '/'
. variable_get('d4os_io_db_multiple_grid_db', 'grid');
$db_prefix['os_grid'] = variable_get('d4os_io_db_multiple_grid_prefix', '');
$d4os_same_db['os_grid'] = variable_get('d4os_io_db_multiple_grid_same', 'none');

// groups
$db_url['os_groups'] = 'mysqli://'
. variable_get('d4os_io_db_multiple_groups_username', 'root')
.':'
. variable_get('d4os_io_db_multiple_groups_password', 'root')
. '@'
. variable_get('d4os_io_db_multiple_groups_host', 'localhost')
. '/'
. variable_get('d4os_io_db_multiple_groups_db', 'groups');
$db_prefix['os_groups'] = variable_get('d4os_io_db_multiple_groups_prefix', '');
$d4os_same_db['os_groups'] = variable_get('d4os_io_db_multiple_groups_same', 'none');

// inventory
//$db_url['os_inventory'] = 'mysqli://'
//. variable_get('d4os_io_db_multiple_inventory_username', 'root')
//.':'
//. variable_get('d4os_io_db_multiple_inventory_password', 'root')
//. '@'
//. variable_get('d4os_io_db_multiple_inventory_host', 'localhost')
//. '/'
//. variable_get('d4os_io_db_multiple_inventory_db', 'inventory');
//$db_prefix['os_inventory'] = variable_get('d4os_io_db_multiple_inventory_prefix', '');
//$d4os_same_db['os_inventory'] = variable_get('d4os_io_db_multiple_inventory_same', 'none');

// messaging
//$db_url['os_messaging'] = 'mysqli://'
//. variable_get('d4os_io_db_multiple_messaging_username', 'root')
//.':'
//. variable_get('d4os_io_db_multiple_messaging_password', 'root')
//. '@'
//. variable_get('d4os_io_db_multiple_messaging_host', 'localhost')
//. '/'
//. variable_get('d4os_io_db_multiple_messaging_db', 'messaging');
//$db_prefix['os_messaging'] = variable_get('d4os_io_db_multiple_messaging_prefix', '');
//$d4os_same_db['os_messaging'] = variable_get('d4os_io_db_multiple_messaging_same', 'none');

// opensim
//$db_url['os_opensim'] = 'mysqli://'
//. variable_get('d4os_io_db_multiple_opensim_username', 'root')
//.':'
//. variable_get('d4os_io_db_multiple_opensim_password', 'root')
//. '@'
//. variable_get('d4os_io_db_multiple_opensim_host', 'localhost')
//. '/'
//. variable_get('d4os_io_db_multiple_opensim_db', 'opensim');
//$db_prefix['os_opensim'] = variable_get('d4os_io_db_multiple_opensim_prefix', '');
//$d4os_same_db['os_opensim'] = variable_get('d4os_io_db_multiple_opensim_same', 'none');

// profile
$db_url['os_profile'] = 'mysqli://'
. variable_get('d4os_io_db_multiple_profile_username', 'root')
.':'
. variable_get('d4os_io_db_multiple_profile_password', 'root')
. '@'
. variable_get('d4os_io_db_multiple_profile_host', 'localhost')
. '/'
. variable_get('d4os_io_db_multiple_profile_db', 'profile');
$db_prefix['os_profile'] = variable_get('d4os_io_db_multiple_profile_prefix', '');
$d4os_same_db['os_profile'] = variable_get('d4os_io_db_multiple_profile_same', 'none');

// search
$db_url['os_search'] = 'mysqli://'
. variable_get('d4os_io_db_multiple_search_username', 'root')
.':'
. variable_get('d4os_io_db_multiple_search_password', 'root')
. '@'
. variable_get('d4os_io_db_multiple_search_host', 'localhost')
. '/'
. variable_get('d4os_io_db_multiple_search_db', 'search');
$db_prefix['os_search'] = variable_get('d4os_io_db_multiple_search_prefix', '');
$d4os_same_db['os_search'] = variable_get('d4os_io_db_multiple_search_same', 'none');

// user_accounts
//$db_url['os_user_accounts'] = 'mysqli://'
//. variable_get('d4os_io_db_multiple_user_accounts_username', 'root')
//.':'
//. variable_get('d4os_io_db_multiple_user_accounts_password', 'root')
//. '@'
//. variable_get('d4os_io_db_multiple_user_accounts_host', 'localhost')
//. '/'
//. variable_get('d4os_io_db_multiple_user_accounts_db', 'user_accounts');
//$db_prefix['os_user_account'] = variable_get('d4os_io_db_multiple_user_account_prefix', '');
//$d4os_same_db['os_account'] = variable_get('d4os_io_db_multiple_account_same', 'none');

// users
$db_url['os_users'] = 'mysqli://'
. variable_get('d4os_io_db_multiple_users_username', 'root')
.':'
. variable_get('d4os_io_db_multiple_users_password', 'root')
. '@'
. variable_get('d4os_io_db_multiple_users_host', 'localhost')
. '/'
. variable_get('d4os_io_db_multiple_users_db', 'users');
$db_prefix['os_users'] = variable_get('d4os_io_db_multiple_users_prefix', '');
$d4os_same_db['os_users'] = variable_get('d4os_io_db_multiple_users_same', 'none');