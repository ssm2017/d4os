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

class D4OS_IO_rest_Users extends REST_Controller {

	function __construct() {
		parent::__construct();
	}
	function ping_get() {
		$this->response(array('status'=>'success'), 200);
	}
  function ping_post() {
    $this->response(array('status'=>'success'), 200);
  }
	function load_user_get() {
    $params = $this->get();

    if (!count($params)) {
      $this->response(NULL, 400);
    }

    // get the users
    $this->load->model('d4os_io_rest_users_model');
    $users = $this->d4os_io_rest_users_model->load_user($params);

    // return the answer
		if (is_array($users) && $users['success'] === TRUE) {
			$this->response($users, 200);
		}
		else {
			$this->response($users, 404);
		}
	}

	function save_user_post() {

    // get values
    if (!$this->post('data')) {
      $this->response('Missing values', 400);
    }
    $data = json_decode($this->post('data'));
    if (empty($data->PrincipalID)) {
      $this->response('Missing values', 400);
    }

    // save user
		$this->load->model('d4os_io_rest_users_model');
		$save_user = $this->d4os_io_rest_users_model->save_user($data);

    // return the answer
		if (is_array($save_user) && $save_user['success'] === TRUE) {
			$this->response($save_user, 200);
		}
		else {
			$this->response($save_user, 400);
		}
	}

	function delete_user_get() {
		if(!$this->get('uuid')) {
			$this->response(NULL, 400);
		}
		$this->load->model('d4os_io_rest_users_model');
		$this->d4os_io_rest_users_model->delete_user($this->get('uuid'));
		$response = array(
			'messages' => $this->d4os_io_rest_users_model->response->messages
		);
		if ($this->d4os_io_rest_users_model->response->success) {
			$response['status'] = 'success';
		}
		else {
			$response['status'] = 'failed';
		}
		$this->response($response, 200);
	}

  function get_online_users_count_get() {
    $online = $this->get('online');
    $range = $this->get('range');
    $this->load->model('d4os_io_rest_users_model');
    $count = $this->d4os_io_rest_users_model->get_online_users_count($online, $range);

		if ($this->d4os_io_rest_users_model->response->success) {
			$this->response($count, 200);
		}
		else {
			$this->response(array('error' => $this->d4os_io_rest_users_model->response->messages), 404);
		}
  }

  function get_online_users_list_get() {
    $online = $this->get('online')? $this->get('online'): FALSE;
    $range = $this->get('range')? $this->get('range'): 0;
    $offset = $this->get('offset')? $this->get('offset'): 0;
    $limit = $this->get('limit')? $this->get('limit'): 5;
    $order_by = $this->get('order_by')? $this->get('order_by'): 'gu.Login';
    $order_way = $this->get('order_way')? $this->get('order_way'): 'DESC';
    $this->load->model('d4os_io_rest_users_model');
    $list = $this->d4os_io_rest_users_model->get_online_users_list($online, $range, $offset, $limit, $order_by, $order_way);
    $list['messages'] = $this->d4os_io_rest_users_model->response->messages;

		if ($this->d4os_io_rest_users_model->response->success) {
			$this->response($list, 200);
		}
		else {
			$this->response(array('error' => $this->d4os_io_rest_users_model->response->messages), 404);
		}
  }
  function test_get() {
    $this->db->select('*');
    echo '<pre>'. print_r($this->db->get_where('UserAccounts', array('FirstName' => 'governor', 'LastName' => 'ssm')), TRUE). '</pre>';
  }
}