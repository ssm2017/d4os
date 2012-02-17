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

class D4OS_IO_rest_Groups extends REST_Controller {

	function __construct() {
		parent::__construct();
	}

	function ping_get() {
		$this->response(array('status'=>'success'), 200);
	}

  function ping_post() {
    $this->response(array('status'=>'success'), 200);
  }

  function group_exists_get() {
    $uuid = $this->get('uuid');
    if (empty($uuid)) {
      $this->response(NULL, 400);
    }
    $this->load->model('d4os_io_rest_groups_model');
    $group_exists = FALSE;
    $group_exists = $this->d4os_io_rest_groups_model->group_exists($uuid);
    if ($group_exists) {
      $this->response(array('status'=>'success'), 200);
    }
    else {
      $this->response(NULL, 404);
    }
  }

  function add_user_to_group_get() {
    $user_uuid = $this->get('user_uuid');
    $group_uuid = $this->get('group_uuid');
    if (empty($user_uuid) || empty($group_uuid)) {
      $this->response(NULL, 400);
    }
    $this->load->model('d4os_io_rest_groups_model');
    $user_added = FALSE;
    $user_added = $this->d4os_io_rest_groups_model->add_user_to_group($user_uuid, $group_uuid);
    if ($user_added) {
      $this->response(array('status'=>'success'), 200);
    }
    else {
      $this->response(NULL, 404);
    }
  }

  function delete_user_get() {
    $uuid = $this->get('uuid');
    if (empty($uuid)) {
      $this->response(NULL, 400);
    }
    $this->load->model('d4os_io_rest_groups_model');
    $user_deleted = FALSE;
    $user_deleted = $this->d4os_io_rest_groups_model->delete_user($uuid);
    if ($user_deleted) {
      $this->response(array('status'=>'success'), 200);
    }
    else {
      $this->response(NULL, 404);
    }
  }
}