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

  function get_region($uuid) {
    d4os_io_db_070_set_active('os_robust');
    $name = db_query("SELECT * FROM {regions} WHERE uuid = :uuid", array(':uuid' => $uuid))->fetch();
    d4os_io_db_070_set_active('default');
    if ($name) {
      return $name;
    }
    return NULL;
  }

  function get_regions_owners() {
    $owners = array();
    d4os_io_db_070_set_active('os_robust');
    $results = db_query('SELECT DISTINCT owner_uuid FROM {regions}');
    foreach ($results as $result) {
      $owners[] = $result->owner_uuid;
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

  function get_regions_list($params) {
    $regions = new stdclass();
    // get region owners
    $owners = $this->get_regions_owners();
    if (count($owners) == 0) {
      return $items;
    }
    // get the drupal uids
    $uids = array();
    $results = db_query("SELECT UUID,uid FROM {d4os_ui_users} WHERE UUID IN('". implode("','", $owners)."')");
    foreach ($results as $result) {
      $uids[$result->UUID] = $result->uid;
    }
    // get regions infos
    $items = array();
    $query = "SELECT r.regionName"
            . ", r.uuid AS region_uuid"
            . ", ROUND(r.locX / 256) AS x"
            . ", ROUND(r.locY / 256) AS y"
            . ", r.serverURI"
            . ", ua.FirstName"
            . ", ua.LastName"
            . ", ua.PrincipalID"
            . " FROM UserAccounts AS ua"
            . " LEFT JOIN regions AS r ON r.owner_uuid=ua.PrincipalID"
            . " WHERE ua.PrincipalID=r.owner_uuid";
    // set where
    if (!is_null($params['uuid'])) {
      $query .= " AND ua.PrincipalID='". $params['uuid']. "'";
    }
    // set ordering
    $query .= " ORDER BY r.regionName";
    $options = array(
      ':order_by' => "r.regionName", //$params['order_by'],
      ':order_way' => "asc"//$params['order_way']
    );
    $test = "";
    d4os_io_db_070_set_active('os_robust');
    $results = db_query_range($query, $params['offset'], $params['limit'])->fetchAll();
    //$test = SelectQuery::__toString();
    foreach ($results as $result) {
      $item = $result;
      $item->uid = isset($uids[$result->PrincipalID]) ? $uids[$result->PrincipalID] : 0;
      $items[] = $item;
    }
    d4os_io_db_070_set_active('default');
    // get the regions count
    $query = "SELECT COUNT(uuid) FROM regions";
    if (!is_null($params['uuid'])) {
      $query .= " WHERE owner_uuid='". $params['uuid']. "'";
    }
    d4os_io_db_070_set_active('os_robust');
    $count = db_query($query)->fetchField();
    d4os_io_db_070_set_active('default');
    $regions->count = $count;
    $regions->regions = $items;
    return $regions;
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

  function get_all_regions() {
    $items = array();
    d4os_io_db_070_set_active('os_robust');
    $results = db_query('SELECT * FROM {regions}');
    foreach ($results as $result) {
      $items[] = $result;
    }
    d4os_io_db_070_set_active('default');
    return $items;
  }

  function delete_region($params) {
    if (isset($params['uuid'])) {
      d4os_io_db_070_set_active('os_robust');
      db_delete('regions')->condition('uuid', $params['uuid'])->execute();
      d4os_io_db_070_set_active('default');
    }
  }
}
