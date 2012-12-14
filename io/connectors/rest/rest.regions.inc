<?php

/**
 * @package    d4os_io rest
 * @copyright Copyright (C) 2010-2012 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @link      http://www.d4os.org
 * @license   GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */
class D4OS_IO_rest_Regions implements D4OS_IO_regions_Interface {

  public $response;
  public $values;

  function __construct() {
    $this->values = array(
    );
  }

  function ping() {
    $this->response->success = FALSE;
    $result = drupal_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_regions/ping/format/json', array('timeout'=> 5));
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      if ($data->status === 'success') {
        $this->response->success = TRUE;
      }
    }
    return;
  }

  function region_exists($uuid) {
    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_regions/region_exists/uuid/' . $uuid . '/format/json';
    $result = drupal_http_request($url, array('timeout'=> 5));
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      return TRUE;
    }
    return FALSE;
  }

  function get_regions_owners() {
    $this->response->success = FALSE;
    $params = '';

    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_regions/get_regions_owners/format/json';
    $result = drupal_http_request($url, array('timeout'=> 5));
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = TRUE;
      if (is_array($data)) {
        return $data;
      }
    }
    return NULL;
  }

  function get_regions_count() {
    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_regions/get_regions_count/format/json';
    $result = drupal_http_request($url, array('timeout'=> 5));
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = TRUE;
      if (is_array($data)) {
        return $data;
      }
    }
    return NULL;
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
    $this->response->success = FALSE;
    $params = '';

    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_regions/get_regions_names/format/json';
    $result = drupal_http_request($url, array('timeout'=> 5));

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = TRUE;
      if (is_array($data)) {
        return $data;
      }
    }
    return NULL;
  }

}
