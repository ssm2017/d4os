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

class D4OS_IO_rest_Grid extends REST_Controller {

	function __construct() {
		parent::__construct();
	}
	function ping_get() {
		$this->response(array('status'=>'success'), 200);
	}
  function ping_post() {
    $this->response(array('status'=>'success'), 200);
  }
  function get_grid_infos_get() {
    $this->load->model('d4os_io_rest_users_model');
    // online now
    $online_now = $this->d4os_io_rest_users_model->get_online_users_count();
    // online last month
    $online_last_month = $this->d4os_io_rest_users_model->get_online_users_count(FALSE, 2419200);
    // users count
    $users_count = $this->d4os_io_rest_users_model->get_users_count();
    // regions count
    $this->load->model('d4os_io_rest_regions_model');
    $regions_count = $this->d4os_io_rest_regions_model->get_regions_count();

    $values = array(
      'online_now' => $online_now,
      'online_last_month' => $online_last_month,
      'users_count' => $users_count,
      'regions_count' => $regions_count
    );
    $this->response($values, 200);
  }
}