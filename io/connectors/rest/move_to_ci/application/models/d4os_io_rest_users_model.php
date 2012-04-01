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

class D4OS_IO_rest_Users_Model extends CI_Model {
	public $response;

	function __construct() {
		parent::__construct();
		$this->response = new stdClass;
		$this->response->messages = array();
		$this->response->success = false;
	}

  function load_user($params) {

    // parse params
    $where = array();
    if (count($params)) {
      foreach ($params as $param => $value) {
        $value = rawurldecode($value);
        switch ($param) {
          case 'PrincipalID':
            $where['ua.PrincipalID'] = $value;
            break;
          case 'ScopeID':
            $where['ua.ScopeID'] = $value;
            break;
          case 'FirstName':
            $where['ua.FirstName'] = $value;
            break;
          case 'LastName':
            $where['ua.LastName'] = $value;
            break;
          case 'Email':
            $where['ua.Email'] = $value;
            break;
          case 'ServiceURLs':
            $where['ua.ServiceURLs'] = $value;
            break;
          case 'Created':
            $where['ua.Created'] = $value;
            break;
          case 'UserLevel':
            $where['ua.UserLevel'] = $value;
            break;
          case 'name':
            $names = explode(' ', $value);
            $where['ua.FirstName'] = $names[0];
            $where['ua.LastName'] = $names[1];
            break;
        }
      }
    }

    if (!count($where)) {
      $this->response->messages[] = 'Missing arguments';
      return array(
        'success' => FALSE,
        'messages' => $this->response->messages
      );
    }

    // get the users
    $users = array();

    $this->db->select('ua.PrincipalID')
      ->select('ua.ScopeID')
      ->select('ua.FirstName')
      ->select('ua.LastName')
      ->select('ua.Email')
      ->select('ua.ServiceURLs')
      ->select('ua.Created')
      ->select('ua.UserLevel')
      ->select('ua.UserFlags')
      ->select('ua.UserTitle')
      ->select('a.passwordHash')
      ->select('a.passwordSalt')
      ->select('a.webLoginKey')
      ->select('a.accountType')
      ->select('gu.HomeRegionID')
      ->select('gu.HomePosition')
      ->select('gu.HomeLookAt')
      ->select('gu.LastRegionID')
      ->select('gu.LastPosition')
      ->select('gu.LastLookAt')
      ->select('gu.Online')
      ->select('gu.Login')
      ->select('gu.Logout')
      ->from('UserAccounts AS ua')
      ->join('auth AS a', 'a.UUID = ua.PrincipalID', 'LEFT')
      ->join('GridUser AS gu', 'gu.UserID = ua.PrincipalID', 'LEFT')
      ->where($where);
    $query = $this->db->get();

		//$query = $this->db->get_where('UserAccounts', $where);

    // build the answer
    if ($query->num_rows() > 0) {
      foreach ($query->result() as $row) {
        $users[] = $row;
      }
      return array(
        'success' => TRUE,
        'messages' => $this->response->messages,
        'data' => $users
      );
    }
    else {
      $this->response->messages[] = 'User not found';
      return array(
        'success' => FALSE,
        'messages' => $this->response->messages
      );
    }
	}

	function save_user($grid_user) {

		// check if the user has a uuid
		if (!isset($grid_user->PrincipalID) || empty($grid_user->PrincipalID)) {
			$this->response->messages[] = 'missing uuid';
      return array(
        'success' => FALSE,
        'messages' => $this->response->messages
      );
		}
		// check if the user already exists
		$user_exists = null;
		if (isset($grid_user->PrincipalID) && !empty($grid_user->PrincipalID)) {
      $user_exists = $this->db->get_where('UserAccounts', array('PrincipalID' => $grid_user->PrincipalID))->num_rows() > 0;
		}

    // check if another user with the same name exists
    /*$username_exists = $this->db->get_where('UserAccounts', array('FirstName' => $grid_user->FirstName, 'LastName' => $grid_user->LastName))->num_rows() > 0;
    if ($username_exists) {
      $this->response->messages[] = 'User with the same name already exists.';
      return array(
        'success' => FALSE,
        'messages' => $this->response->messages
      );
    }*/

		if ($user_exists) {

			// update the UserAccounts table
      $values = $this->get_useraccount_values($grid_user);
      if (count($values)) {
        $this->db->where('PrincipalID', $grid_user->PrincipalID);
        $this->db->update('UserAccounts', $values);
        $return = $this->db->_error_number()==0;
        if ($return) {
          $this->response->messages[] = 'UserAccount updated';
        }
        else {
          $this->response->messages[] = $return;
          return array(
            'success' => FALSE,
            'messages' => $this->response->messages
          );
        }
      }

			// update the auth table
      $values = $this->get_auth_values($grid_user);
      if (count($values)) {
        $this->db->where('UUID', $grid_user->PrincipalID);
        $this->db->update('auth', $values);
        $return = $this->db->_error_number()==0;
        if ($return) {
          $this->response->messages[] = 'auth updated';
        }
        else {
          $this->response->messages[] = $return;
          return array(
            'success' => FALSE,
            'messages' => $this->response->messages
          );
        }
      }

			// update the GridUser table
      $values = $this->get_griduser_values($grid_user);
      if (count($values)) {
        $this->db->where('UserID', $grid_user->PrincipalID);
        $this->db->update('GridUser', $values);
        $return = $this->db->_error_number()==0;
        if ($return) {
          $this->response->messages[] = 'GridUser updated';
        }
        else {
          $this->response->messages[] = $return;
          return array(
            'success' => FALSE,
            'messages' => $this->response->messages
          );
        }
      }
		}
		else {
      // user does not exist so create a new one

			// check if we have the minimum requirements
			if (!isset(
        $grid_user->PrincipalID ,
        $grid_user->FirstName ,
        $grid_user->LastName ,
        $grid_user->Email ,
        $grid_user->passwordSalt ,
        $grid_user->passwordHash
			)) {
				$this->response->messages[] = 'Missing fields.';
				return array(
          'success' => FALSE,
          'messages' => $this->response->messages
        );
			}

			// insert in the UserAccounts table
      $values = $this->get_useraccount_values($grid_user);
      if (count($values)) {
        // add blank value to "ScopeID"
        if (!isset($grid_user->ScopeID)) {
          $values['ScopeID'] = UUID_ZERO;
        }
        if (!isset($grid_user->ServiceURLs)) {
          $this->config->load('d4os_io_rest');
          $values['ServiceURLs'] = $this->config->item('d4os_io_rest_user_default_ServiceURLs');
        }
        $values['PrincipalID'] = $grid_user->PrincipalID;
        $this->db->insert('UserAccounts', $values);

        $return = $this->db->_error_number()==0;
        if ($return) {
          $this->response->messages[] = 'User added to UserAccounts';
        }
        else {
          $this->response->messages[] = $return;
          return array(
            'success' => FALSE,
            'messages' => $this->response->messages
          );
        }
      }

			// insert in the auth table
      $values = $this->get_auth_values($grid_user);
      if (count($values)) {
        $this->db->insert('auth', $values);
        $return = $this->db->_error_number()==0;
        if ($return) {
          $this->response->messages[] = 'User added to auth';
        }
        else {
          $this->response->messages[] = $return;
          return array(
            'success' => FALSE,
            'messages' => $this->response->messages
          );
        }
      }

			// insert in the GridUser table
      $values = $this->get_griduser_values($grid_user);
      if (count($values)) {
        $this->db->insert('GridUser', $values);
        $return = $this->db->_error_number()==0;
        if ($return) {
          $this->response->messages[] = 'User added to GridUser';
        }
        else {
          $this->response->messages[] = $return;
          return array(
            'success' => FALSE,
            'messages' => $this->response->messages
          );
        }
      }

      // create new inventory or clone a model
      $this->load->model('d4os_io_rest_inventory_model');
      if (isset($grid_user->defaultModel)) {
        if ($grid_user->defaultModel != UUID_ZERO) {
          $avatar_src_uuid = $grid_user->defaultModel;
          $this->response->messages[] = 'Creating avatar using model : '. $avatar_src_uuid;
          $params = array(
            'avatar_src_uuid' => $avatar_src_uuid,
            'avatar_dest_uuid' => $grid_user->PrincipalID,
          );
          $cloned_inventory = $this->d4os_io_rest_inventory_model->clone_inventory($params);
          $this->response->messages[] = $cloned_inventory['message'];
          $cloned_appearance = $this->d4os_io_rest_inventory_model->clone_appearance($params);
          $this->response->messages[] = $cloned_appearance['message'];
        }
        else {
          $this->d4os_io_rest_inventory_model->create_new_inventory(array('user_uuid' => $grid_user->PrincipalID));
        }
      }
      else {
        $this->d4os_io_rest_inventory_model->create_new_inventory(array('user_uuid' => $grid_user->PrincipalID));
      }
		}

		// Refresh user object.
		$user = $this->load_user(array('PrincipalID' => $grid_user->PrincipalID));
		if (is_array($user) && $user['success'] == TRUE) {
			$this->response->success = true;
			return $user;
		}
    else {
      $this->response->messages[] = 'Error loading final user';
      return array(
        'success' => FALSE,
        'messages' => $this->response->messages
      );
    }
	}

  function get_useraccount_values($grid_user) {
    $values = array();

    // parse values
    foreach ($grid_user as $key => $value) {
      switch ($key) {
        //case 'PrincipalID':
          //$values['PrincipalID'] = $value;
          //break;
        case 'ScopeID':
          $values['ScopeID'] = $value;
          break;
        case 'FirstName':
          $values['FirstName'] = $value;
          break;
        case 'LastName':
          $values['LastName'] = $value;
          break;
        case 'Email':
          $values['Email'] = $value;
          break;
        case 'ServiceURLs':
          $values['ServiceURLs'] = $value;
          break;
        case 'Created':
          $values['Created'] = $value;
          break;
        case 'UserLevel':
          $values['UserLevel'] = $value;
          break;
        case 'UserTitle':
          $values['UserTitle'] = $value;
          break;
      }
    }
    return $values;
  }

  function get_auth_values($grid_user) {
    $values   = array('UUID'=> $grid_user->PrincipalID);
    foreach ($grid_user as $key => $value) {
      if (!empty($value)) {
        switch ($key) {
          case 'passwordSalt':
            $values['passwordSalt'] = $value;
            break;
          case 'passwordHash':
            $values['passwordHash'] = $value;
            break;
          case 'webLoginKey':
            $values['webLoginKey'] = $value;
            break;
          case 'accountType':
            $values['accountType'] = $value;
            break;
        }
      }
    }
    return $values;
  }

  function get_griduser_values($grid_user) {
    $values   = array('UserID'=> $grid_user->PrincipalID);

    foreach ($grid_user as $key => $value) {
      if (!empty($value)) {
        switch ($key) {
          case 'HomeRegionID':
            $values['HomeRegionID'] = $value;
            break;
          case 'HomePosition':
            $values['HomePosition'] = $value;
            break;
          case 'HomeLookAt':
            $values['HomeLookAt'] = $value;
            break;
          case 'LastRegionID':
            $values['LastRegionID '] = $value;
            break;
          case 'LastPosition':
            $values['LastPosition '] = $value;
            break;
          case 'LastLookAt':
            $values['LastLookAt '] = $value;
            break;
          case 'Login':
            $values['Login'] = $value;
            break;
        }
      }
    }
    return $values;
  }

	function delete_user($uuid) {
		// delete from UserAccounts
		$this->db->delete('UserAccounts', array('PrincipalID' => $uuid));
		// delete from auth
		$this->db->delete('auth', array('UUID' => $uuid));
		// delete from GridUser
		$this->db->delete('GridUser', array('UserID' => $uuid));
    // delete from Presence
		$this->db->delete('Presence', array('UserID' => $uuid));
    // delete from Tokens
		$this->db->delete('tokens', array('UUID' => $uuid));
		// delete from Friends
    $this->db->or_where(array('PrincipalID' => $uuid, 'Friend' => $uuid));
		$this->db->delete('Friends');
    $this->response->messages[] = 'User '. $uuid. ' deleted from grid.';
		// delete inventory
		$this->load->model('d4os_io_rest_inventory_model');
    $this->d4os_io_rest_inventory_model->delete_user_inventory($uuid);
		$this->response->messages = array_merge($this->response->messages,$this->d4os_io_rest_inventory_model->response->messages);
		$this->response->success = true;
	}

  function get_online_users_count($online = FALSE, $range = 0) {
    // day = 86400 / month = 2419200 / year = 29030400
    $this->db->from('GridUser');
    if (!$range || !is_numeric($range)) {
      $this->db->where('Online', 'True');
    }
    else {
      if ($online) {
        $this->db->where('Online', 'True');
      }
      $this->db->where('Login >', time() - $range);
    }
    $count = $this->db->count_all_results();
    $this->response->success = TRUE;
    $this->response->data = $count;
    //return $this->db->last_query();
    return $count;
  }

  function get_users_count() {
    $count = $this->db->count_all('UserAccounts');
    $this->response->success = TRUE;
    $this->response->data = $count;
    //return $this->db->last_query();
    return $count;
  }

  function get_online_users_list($online = FALSE, $range = 0, $offset = 0, $limit = 10, $order_by = 'gu.Login', $order_way = 'DESC') {
    $users = array();

    // get the quantity of users in Presence table
    $this->get_online_users_list_select($online, $range);
    $users['count_presence'] = $this->db->count_all_results();
    $this->get_online_users_list_select($online, $range);
    $this->db->limit($limit, $offset);
    $this->db->order_by($order_by, $order_way);

    $query = $this->db->get();

    if ($query->num_rows() > 0) {
      foreach ($query->result() as $row) {
        $users['users'][] = $row;
      }
    }

    // get the quantity of users "online" in the GridUser table
    $users['count_griduser'] = $this->get_online_users_count();

    $this->response->success = TRUE;
    $this->response->messages[] = $this->db->last_query();
    $this->response->data = $users;
    return $users;
  }

  function get_online_users_list_select($online, $range) {
    $this->db->select('p.UserID')
              ->select('ua.FirstName')
              ->select('ua.LastName')
              ->select('r.regionName')
              ->select('gu.Login')
              ->select('gu.Logout')
              ->select('gu.LastPosition')
              ->from('Presence AS p')
              ->join('UserAccounts AS ua', 'ua.PrincipalID = p.UserID', 'LEFT')
              ->join('regions AS r', 'r.uuid = p.RegionID', 'LEFT')
              ->join('GridUser AS gu', 'gu.UserID = p.UserID', 'LEFT');

    if ($online) {
      $this->db->where('gu.Online', 'True');
    }
    if ($range) {
      $this->db->where('gu.Login >', time() - $range);
    }
  }
}