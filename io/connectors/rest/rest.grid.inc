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
class D4OS_IO_rest_Grid implements D4OS_IO_Grid_Interface {

  public $response;
  public $values;

  function __construct() {
    $this->response = new stdClass();
    $this->values = array();
  }

  function ping() {
    $this->response->success = FALSE;
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_grid/ping/format/json');
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      if ($data->status === 'success') {
        $this->response->success = TRUE;
      }
    }
    return;
  }

  function get_grid_infos() {
    $this->response->success = FALSE;
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_grid/get_grid_infos/format/json');
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = TRUE;
      if (is_object($data)) {
        return $data;
      }
    }
    return NULL;
  }

}
