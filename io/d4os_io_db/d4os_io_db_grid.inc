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
  db_set_active('os_grid');
  $result = db_query('SELECT * FROM {regions} WHERE '. implode(' AND ', $query), $params);
  $region = db_fetch_object($result);
  db_set_active('default');
  return $region;
}

function d4os_io_db_os_regions_list_paged($page = 0, $limit = 5) {
  if (!isset($_GET['page'])) {
    $_GET['page'] = $page;
  }
  if (!isset($_GET['order'])) {
    $_GET['order'] = 'regionName';
  }
  if (!isset($_GET['sort'])) {
    $_GET['sort'] = 'DESC';
  }
  db_set_active('os_grid');
  $result = pager_query('SELECT * FROM {regions} ORDER BY %s %s', $limit, 0, NULL, array('order'=>$_GET['order'], 'sort'=>$_GET['sort']));
  while ($region = db_fetch_object($result)) {
    $items[] = $region;
  }
  db_set_active('default');
  return $items;
}

function d4os_io_db_os_regions_names() {
  db_set_active('os_grid');
  $result = db_query('SELECT uuid, regionHandle, regionName FROM {regions} ORDER BY regionName');
  while ($region = db_fetch_object($result)) {
    $items[] = $region;
  }
  db_set_active('default');
  return $items;
}