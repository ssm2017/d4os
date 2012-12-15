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
class D4OS_IO_db_070_Regions implements D4OS_IO_regions_Interface {

  public $response;
  public $values;

  function __construct() {
    $this->response = new stdClass();
    $this->values = array();
  }

  function ping() {
    $this->response->success = d4os_io_db_070_mysql_is_alive('os_robust', 'Robust');
    return;
  }

  function get_regions_owners() {
    $owners = array();
    d4os_io_db_070_set_active('os_robust');
    $results = db_query('SELECT DISTINCT owner_uuid FROM {regions}');
    foreach ($results as $result) {
      $owners[] = $result;
    }
    d4os_io_db_070_set_active('default');
    return $owners;
  }

  function region_exists($uuid) {
    d4os_io_db_070_set_active('os_robust');
    $region_exists = db_query("SELECT count(regionName) FROM {regions} WHERE uuid = :uuid", array(':uuid' => $uuid))->fetchField();
    d4os_io_db_070_set_active('default');
    if ($region_exists) {
      return TRUE;
    }
    return FALSE;
  }

  function get_regions_count() {
    d4os_io_db_070_set_active('os_robust');
    $count = db_query("SELECT COUNT(uuid) FROM {regions}")->fetchField();
    d4os_io_db_070_set_active('default');
    return $count;
  }

  /**
   * Return a list of regions for a html list in the events module
   * @return Array An array of regions like $regions_list[$handle] = $region->regionName;
   */
  function get_regions_array() {
    $regions = $this->get_regions_names();
    $regions_list = array();
    if (count($regions)) {
      foreach ($regions as $region) {
        $handle = '_' . $region->uuid;
        $regions_list[$handle] = $region->regionName;
      }
    }
    return $regions_list;
  }

  /**
   * Return a list of regions objects ordered by regionName
   * @return Array An array of regions objects
   */
  function get_regions_names() {
    $items = array();
    d4os_io_db_070_set_active('os_robust');
    $results = db_query('SELECT uuid, regionHandle, regionName FROM {regions} ORDER BY regionName');
    foreach ($results as $result) {
      $items[] = $result;
    }
    d4os_io_db_070_set_active('default');
    return $items;
  }

}
