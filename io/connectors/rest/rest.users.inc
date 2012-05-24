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

class D4OS_IO_rest_Users implements D4OS_IO_Users_Interface {
	public $response;
	public $values;

	function __construct() {
		$this->values = array(

		);
	}

	function ping() {
		$this->response->success = false;
		$result = d4os_http_request(D4OS_IO_REST_URL. '/index.php/d4os_io_rest_users/ping/format/json');
		if(isset($result) && in_array($result->code, array('200')) && $result->data != '') {
			$data = json_decode($result->data);
			if ($data->status === 'success') {
				$this->response->success = true;
			}
		}
		return;
	}

	function load_user($values) {
    $this->response->success = false;
    $params = '';

    // parse values
    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key. '/'. rawurlencode($value). '/';
      }
    }

    // parse url
		$url = D4OS_IO_REST_URL. '/index.php/d4os_io_rest_users/load_user/'. $params. 'format/json';

    // request the server
		$result = d4os_http_request($url);

    // parse the answer
		if(isset($result) && in_array($result->code, array('200')) && $result->data != '') {
			$data = json_decode($result->data);
      if (is_array($data->data)) {
        $this->response->success = true;
        return $data->data;
      }
		}
		return null;
	}

	function save_user($grid_user) {
    $data = array("data" => json_encode($grid_user));
    $response = d4os_http_request(D4OS_IO_REST_URL. '/index.php/d4os_io_rest_users/save_user/format/json'
        , array('Content-Type' => 'application/x-www-form-urlencoded')
        , 'POST', http_build_query($data, '', '&'));
    $data = json_decode($response->data);
    return $data->data;
  }

	function delete_user($uuid) {
    $this->response->success = false;
		$result = d4os_http_request(D4OS_IO_REST_URL. '/index.php/d4os_io_rest_users/delete_user/uuid/'. rawurlencode($uuid). '/format/json');
		if(isset($result) && in_array($result->code, array('200')) && $result->data != '') {
			$data = json_decode($result->data);
			if ($data->status === 'success') {
				$this->response->success = true;
        if (is_array($data->messages)) {
          foreach ($data->messages as $message) {
            drupal_set_message($message);
          }
        }
			}
		}
		return;
	}

  function get_online_users_list($values) {
    $this->response->success = false;
    $params = '';
    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key. '/'. rawurlencode($value). '/';
      }
    }

		$url = D4OS_IO_REST_URL. '/index.php/d4os_io_rest_users/get_online_users_list/'. $params. '/format/json';
		$result = d4os_http_request($url);
		if(isset($result) && in_array($result->code, array('200')) && $result->data != '') {
			$data = json_decode($result->data);
      $this->response->success = true;
      if (is_object($data)) {
        return $data;
      }
		}
		return null;
	}

  /**
  * Assign a grid uuid to a Drupal user id
  * @param Integer Drupal user id
  * @param Integer Grid user uuid
  */
  function set_uuid($uid, $uuid) {
    // check the link between drupal and the grid
    $uid_exists = db_query("SELECT uid FROM {d4os_ui_users} WHERE UUID=:uuid OR uid=:uid", array(':uuid' => $uuid, ':uid' => $uid))->fetchField();
    if ($uid_exists) {
      db_query("DELETE FROM {d4os_ui_users} WHERE UUID = :uuid OR uid = :uid", array(':uuid' => $uuid, ':uid' => $uid));
    }
    db_query("INSERT INTO {d4os_ui_users} (UUID, uid) VALUES (:uuid, :uid)", array(':uuid' => $uuid, ':uid' => $uid));
  }
}
