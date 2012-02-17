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

require APPPATH.'/libraries/REST_Controller.php';

class D4OS_IO_rest_Regions extends REST_Controller {

	function __construct() {
		parent::__construct();
	}

	function ping_get() {
		$this->response(array('status'=>'success'), 200);
	}

  function ping_post() {
    $this->response(array('status'=>'success'), 200);
  }

  function region_exists_get() {
    $uuid = $this->get('uuid');
    if (empty($uuid)) {
      $this->response(NULL, 400);
    }
    $this->load->model('d4os_io_rest_regions_model');
    $region_exists = FALSE;
    $region_exists = $this->d4os_io_rest_regions_model->region_exists($uuid);
    if ($region_exists) {
      $this->response(array('status'=>'success'), 200);
    }
    else {
      $this->response(NULL, 404);
    }
  }

  function get_regions_owners_get() {
    $this->load->model('d4os_io_rest_regions_model');
    $regions = $this->d4os_io_rest_regions_model->get_regions_owners();

		if ($this->d4os_io_rest_regions_model->response->success) {
			$this->response($regions, 200);
		}
		else {
			$this->response(array('error' => $this->d4os_io_rest_regions_model->response->messages), 404);
		}
  }

  function get_regions_count_get() {
    $this->load->model('d4os_io_rest_regions_model');
    $count = $this->d4os_io_rest_regions_model->get_regions_count();

		if ($this->d4os_io_rest_regions_model->response->success) {
			$this->response($count, 200);
		}
		else {
			$this->response(array('error' => $this->d4os_io_rest_regions_model->response->messages), 404);
		}
  }

  function get_regions_names_get() {
    $this->load->model('d4os_io_rest_regions_model');
    $regions = $this->d4os_io_rest_regions_model->get_regions_names();

		if ($this->d4os_io_rest_regions_model->response->success) {
			$this->response($regions, 200);
		}
		else {
			$this->response(array('error' => $this->d4os_io_rest_regions_model->response->messages), 404);
		}
  }
}