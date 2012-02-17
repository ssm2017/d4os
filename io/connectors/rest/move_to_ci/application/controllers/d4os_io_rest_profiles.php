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

class D4OS_IO_rest_Profiles extends REST_Controller {

	function __construct() {
		parent::__construct();
	}
	function ping_get() {
		$this->response(array('status'=>'success'), 200);
	}
  function ping_post() {
    $this->response(array('status'=>'success'), 200);
  }
  function load_profile_get() {
    $params = $this->get();
    $where = array();
    if (count($params)) {
      foreach ($params as $param => $value) {
        $value = rawurldecode($value);
        switch ($param) {
          case 'useruuid':
            $where['useruuid'] = $value;
            break;
          case 'profileSkillsMask':
            $where['profileSkillsMask'] = $value;
            break;
          case 'profileWantToMask':
            $where['profileWantToMask'] = $value;
            break;
          case 'profileAboutText':
            $where['profileAboutText'] = $value;
            break;
          case 'profileFirstText':
            $where['profileFirstText'] = $value;
            break;
          case 'profileImage':
            $where['profileImage'] = $value;
            break;
          case 'profileFirstImage':
            $where['profileFirstImage'] = $value;
            break;
          case 'profilePartner':
            $where['profilePartner'] = $value;
            break;
        }
      }
    }
    else {
      $this->response(NULL, 400);
    }
    if (!count($where)) {
      $this->response(NULL, 400);
    }
    $this->load->model('d4os_io_rest_profiles_model');
    $profiles = $this->d4os_io_rest_profiles_model->get_profile($where);

		if ($profiles['success']) {
			$this->response($profiles['data'], 200);
		}
		else {
			$this->response(array('error' => $profiles['errorMessage']), 404);
		}
  }

  function save_profile_post() {
    // get values
    if (!$this->post('data')) {
      $this->response('Missing values', 400);
    }
    $data = json_decode($this->post('data'));

		$this->load->model('d4os_io_rest_profiles_model');
		$updated = $this->d4os_io_rest_profiles_model->save_profile((array)$data);

    // return the answer
		if ($updated['success'] === TRUE) {
			$this->response($updated['errorMessage'], 200);
		}
		else {
			$this->response($updated['errorMessage'], 400);
		}
  }

  function delete_profile_get() {
    $useruuid = $this->get('useruuid');
    if (empty($useruuid)) {
      $this->response(NULL, 400);
    }
    $this->load->model('d4os_io_rest_profiles_model');
    $user_deleted = FALSE;
    $user_deleted = $this->d4os_io_rest_profiles_model->delete_profile($useruuid);

    if ($user_deleted['success']) {
      $this->response($user_deleted['errorMessage'], 200);
    }
    else {
      $this->response($user_deleted['errorMessage'], 404);
    }
  }

  /*
  * Profile services
  */
  function avatarclassifiedsrequest_get() {
    $where = (array)$this->get();
    $this->load->model('d4os_io_rest_profiles_model');
    $data = $this->d4os_io_rest_profiles_model->avatarclassifiedsrequest($where);

		if ($data['success']) {
			$this->response($data['data'], 200);
		}
		else {
			$this->response(array('error' => $data['errorMessage']), 404);
		}
  }

  function classified_update_post() {
    // get values
    if (!$this->post('data')) {
      $this->response('Missing values', 400);
    }
    $data = json_decode($this->post('data'));

		$this->load->model('d4os_io_rest_profiles_model');
		$updated = $this->d4os_io_rest_profiles_model->classified_update((array)$data);

    // return the answer
		if ($updated['success'] === TRUE) {
			$this->response($updated['errorMessage'], 200);
		}
		else {
			$this->response($updated['errorMessage'], 400);
		}
  }

  function classified_delete_get() {
    $id = $this->get('classifiedID');
    if (empty($id)) {
      $this->response(NULL, 400);
    }
    $this->load->model('d4os_io_rest_profiles_model');
    $deleted = $this->d4os_io_rest_profiles_model->classified_delete($id);
    if ($deleted['success']) {
      $this->response(array('status'=>'success'), 200);
    }
    else {
      $this->response(NULL, 404);
    }
  }

  function avatarpicksrequest_get() {
    $where = (array)$this->get();
    $this->load->model('d4os_io_rest_profiles_model');
    $data = $this->d4os_io_rest_profiles_model->avatarpicksrequest($where);

		if ($data['success']) {
			$this->response($data['data'], 200);
		}
		else {
			$this->response(array('error' => $data['errorMessage']), 404);
		}
  }

  function pickinforequest_get() {
    $where = (array)$this->get();
    $this->load->model('d4os_io_rest_profiles_model');
    $data = $this->d4os_io_rest_profiles_model->pickinforequest($where);

		if ($data['success']) {
			$this->response($data['data'], 200);
		}
		else {
			$this->response(array('error' => $data['errorMessage']), 404);
		}
  }

  function picks_update_post() {
    // get values
    if (!$this->post('data')) {
      $this->response('Missing values', 400);
    }
    $data = json_decode($this->post('data'));

		$this->load->model('d4os_io_rest_profiles_model');
		$updated = $this->d4os_io_rest_profiles_model->picks_update((array)$data);

    // return the answer
		if ($updated['success'] === TRUE) {
			$this->response($updated['errorMessage'], 200);
		}
		else {
			$this->response($updated['errorMessage'], 400);
		}
  }

  function picks_delete_get() {
    $id = $this->get('pick_id');
    if (empty($id)) {
      $this->response(NULL, 400);
    }
    $this->load->model('d4os_io_rest_profiles_model');
    $deleted = $this->d4os_io_rest_profiles_model->picks_delete($id);
    if ($deleted['success']) {
      $this->response(array('status'=>'success'), 200);
    }
    else {
      $this->response(NULL, 404);
    }
  }

  function avatarnotesrequest_get() {
    $where = (array)$this->get();
    $this->load->model('d4os_io_rest_profiles_model');
    $data = $this->d4os_io_rest_profiles_model->avatarnotesrequest($where);

		if ($data['success']) {
			$this->response($data['data'], 200);
		}
		else {
			$this->response(array('error' => $data['errorMessage']), 404);
		}
  }

  function avatar_notes_update_post() {
    // get values
    if (!$this->post('data')) {
      $this->response('Missing values', 400);
    }
    $data = json_decode($this->post('data'));

		$this->load->model('d4os_io_rest_profiles_model');
		$updated = $this->d4os_io_rest_profiles_model->avatar_notes_update((array)$data);

    // return the answer
		if ($updated['success'] === TRUE) {
			$this->response($updated['errorMessage'], 200);
		}
		else {
			$this->response($updated['errorMessage'], 400);
		}
  }

  function avatar_properties_request_get() {
    $where = (array)$this->get();
    $this->load->model('d4os_io_rest_profiles_model');
    $data = $this->d4os_io_rest_profiles_model->avatar_properties_request($where);

		if ($data['success']) {
			$this->response($data['data'], 200);
		}
		else {
			$this->response(array('error' => $data['errorMessage']), 404);
		}
  }

  function avatar_properties_update_post() {
    // get values
    if (!$this->post('data')) {
      $this->response('Missing values', 400);
    }
    $data = json_decode($this->post('data'));

		$this->load->model('d4os_io_rest_profiles_model');
		$updated = $this->d4os_io_rest_profiles_model->avatar_properties_update((array)$data);

    // return the answer
		if ($updated['success'] === TRUE) {
			$this->response($updated['errorMessage'], 200);
		}
		else {
			$this->response($updated['errorMessage'], 400);
		}
  }

  function avatar_interests_request_get() {
    $where = (array)$this->get();
    $this->load->model('d4os_io_rest_profiles_model');
    $data = $this->d4os_io_rest_profiles_model->avatar_interests_request($where);

		if ($data['success']) {
			$this->response($data['data'], 200);
		}
		else {
			$this->response(array('error' => $data['errorMessage']), 404);
		}
  }

  function avatar_interests_update_post() {
    // get values
    if (!$this->post('data')) {
      $this->response('Missing values', 400);
    }
    $data = json_decode($this->post('data'));

		$this->load->model('d4os_io_rest_profiles_model');
		$updated = $this->d4os_io_rest_profiles_model->avatar_interests_update((array)$data);

    // return the answer
		if ($updated['success'] === TRUE) {
			$this->response($updated['errorMessage'], 200);
		}
		else {
			$this->response($updated['errorMessage'], 400);
		}
  }

  function user_preferences_request_get() {
    $where = (array)$this->get();
    $this->load->model('d4os_io_rest_profiles_model');
    $data = $this->d4os_io_rest_profiles_model->user_preferences_request($where);

		if ($data['success']) {
			$this->response($data['data'], 200);
		}
		else {
			$this->response(array('error' => $data['errorMessage']), 404);
		}
  }

  function user_preferences_update_post() {
    // get values
    if (!$this->post('data')) {
      $this->response('Missing values', 400);
    }
    $data = json_decode($this->post('data'));

		$this->load->model('d4os_io_rest_profiles_model');
		$updated = $this->d4os_io_rest_profiles_model->user_preferences_update((array)$data);

    // return the answer
		if ($updated['success'] === TRUE) {
			$this->response($updated['errorMessage'], 200);
		}
		else {
			$this->response($updated['errorMessage'], 400);
		}
  }
}
