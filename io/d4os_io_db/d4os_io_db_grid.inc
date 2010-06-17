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

function d4os_io_db_os_region_load($array = array()) {
  // Dynamically compose a SQL query:
  $query = array();
  $params = array();

  if (!is_array($array)) {
    return FALSE;
  }

  foreach ($array as $key => $value) {
    $query[]= "LOWER($key) = LOWER('%s')";
    $params[] = $value;
  }
  d4os_io_db_set_active('os_grid');
  $result = db_query('SELECT * FROM {regions} WHERE '. implode(' AND ', $query), $params);
  $region = db_fetch_object($result);
  d4os_io_db_set_active('default');
  return $region;
}

function d4os_io_db_os_regions_list_paged($page = 0, $limit = 5, $headers) {
  if (!isset($_GET['page'])) {
    $_GET['page'] = $page;
  }
  if (!isset($_GET['order'])) {
    $_GET['order'] = 'regionName';
  }
  if (!isset($_GET['sort'])) {
    $_GET['sort'] = 'DESC';
  }
  d4os_io_db_set_active('os_grid');
  $result = pager_query('SELECT * FROM {regions} %s', $limit, 0, NULL, array(tablesort_sql($headers)));
  while ($region = db_fetch_object($result)) {
    $items[] = $region;
  }
  d4os_io_db_set_active('default');
  return $items;
}

function d4os_io_db_os_regions_regions_count() {
  d4os_io_db_set_active('os_grid');
  $count = db_result(db_query("SELECT COUNT(*) FROM {regions}"));
  d4os_io_db_set_active('default');
  return $count;
}

function d4os_io_db_os_regions_get_regions_array() {
  $regions = d4os_io_db_os_regions_names();
  $regions_list = array();
  foreach ($regions as $region) {
    $handle = $region->regionHandle;
    $regions_list[$handle] = $region->regionName;
  }
  return $regions_list;
}

function d4os_io_db_os_regions_names() {
  d4os_io_db_set_active('os_grid');
  $result = db_query('SELECT uuid, regionHandle, regionName FROM {regions} ORDER BY regionName');
  while ($region = db_fetch_object($result)) {
    $items[] = $region;
  }
  d4os_io_db_set_active('default');
  return $items;
}

function d4os_io_db_os_grid_get_stats() {
  // who is online now
  $now_online = 0;
  $online_users = array();
  d4os_io_db_set_active('os_users');
  $users = db_query("SELECT UUID, currentRegion FROM {agents}"
                            . " WHERE agentOnline = 1"
                            . " AND logintime < (UNIX_TIMESTAMP(FROM_UNIXTIME(UNIX_TIMESTAMP(now()))))"
                            . " AND logoutTime < (UNIX_TIMESTAMP(FROM_UNIXTIME(UNIX_TIMESTAMP(now()))))"
                            . " ORDER BY logintime DESC");
  while($online_user = db_fetch_object($users)) {
    $online_users[] = $online_user;
  }
  d4os_io_db_set_active('default');

  // get the regions
  $regions_list = array();
  d4os_io_db_set_active('os_grid');
  $regions = db_query("SELECT regionName, UUID FROM {regions}");
  while ($region = db_fetch_object($regions)) {
    $regions_list[] = $region;
  }
  d4os_io_db_set_active('default');

  // check if the user is in a valid region
  foreach ($online_users as $online_user) {
    if (d4os_array_search_recursive($online_user->currentRegion, $regions_list)) {
      ++$now_online;
    }
  }

  // last month online
  d4os_io_db_set_active('os_users');
  $last_month_online = db_result(db_query("SELECT count(*) FROM {agents} WHERE logintime > UNIX_TIMESTAMP(FROM_UNIXTIME(UNIX_TIMESTAMP(now()) - 2419200))"));
  d4os_io_db_set_active('default');

  // users count
  d4os_io_db_set_active('os_users');
  $users_count = db_result(db_query("SELECT count(*) FROM users"));
  d4os_io_db_set_active('default');

  // regions count
  d4os_io_db_set_active('os_grid');
  $regions_count = db_result(db_query("SELECT count(*) FROM regions"));
  d4os_io_db_set_active('default');

  $values = new stdClass;
  $values->now_online         = $now_online;
  $values->last_month_online  = $last_month_online;
  $values->users_count        = $users_count;
  $values->regions_count      = $regions_count;
  return array(
    'success' => TRUE,
    'data'    => $values,
  );
}

function d4os_io_db_os_grid_is_alive() {
  return array(
    'success' => d4os_io_db_mysql_is_alive('os_grid', 'os grid')
  );
}