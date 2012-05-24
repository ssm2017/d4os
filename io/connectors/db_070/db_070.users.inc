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

class D4OS_IO_db_070_Users implements D4OS_IO_Users_Interface {
	public $response;
	public $values;

	function ping() {
		$this->response->success = d4os_io_db_070_mysql_is_alive('os_robust', 'Robust');
		return;
	}

	function load_user($params) {

    $this->response->success = false;
    // Dynamically compose a SQL query:
    $queries = array();
    $values = array();
    if (count($params)) {
      foreach ($params as $param => $value) {
        $value = rawurldecode($value);
        switch ($param) {
          case 'PrincipalID':
            $queries[]= "ua.PrincipalID = :PrincipalID";
            $values[':PrincipalID'] = $value;
            break;
          case 'ScopeID':
            $queries[]= "ua.ScopeID = :ScopeID";
            $values[':ScopeID'] = $value;
            break;
          case 'FirstName':
            $queries[]= "ua.FirstName = :FirstName";
            $values[':FirstName'] = $value;
            break;
          case 'LastName':
            $queries[]= "ua.LastName = :LastName";
            $values[':LastName'] = $value;
            break;
          case 'Email':
            $queries[]= "ua.Email = :Email";
            $values[':Email'] = $value;
            break;
          case 'ServiceURLs':
            $queries[]= "ua.ServiceURLs = :ServiceURLs";
            $values[':ServiceURLs'] = $value;
            break;
          case 'Created':
            $queries[]= "ua.Created = :Created";
            $values[':Created'] = $value;
            break;
          case 'UserLevel':
            $queries[]= "ua.UserLevel = :UserLevel";
            $values[':UserLevel'] = $value;
            break;
          case 'name':
            $names = explode(' ', $value);
            $queries[]= "ua.FirstName = :FirstName";
            $values[':FirstName'] = $names[0];
            $queries[]= "ua.LastName = :LastName";
            $values[':LastName'] = $names[1];
            break;
        }
      }
    }
    else {
      return NULL;
    }
    if (!count($queries)) {
      return NULL;
    }

    $users = array();
    $query = "SELECT ua.PrincipalID"
            . ", ua.ScopeID"
            . ", ua.FirstName"
            . ", ua.LastName"
            . ", ua.Email"
            . ", ua.ServiceURLs"
            . ", ua.Created"
            . ", ua.UserLevel"
            . ", ua.UserFlags"
            . ", ua.UserTitle"
            . ", a.passwordHash"
            . ", a.passwordSalt"
            . ", a.webLoginKey"
            . ", a.accountType"
            . ", gu.HomeRegionID"
            . ", gu.HomePosition"
            . ", gu.HomeLookAt"
            . ", gu.LastRegionID"
            . ", gu.LastPosition"
            . ", gu.LastLookAt"
            . ", gu.Online"
            . ", gu.Login"
            . ", gu.Logout"
            . " FROM {UserAccounts} AS ua"
            . " LEFT JOIN {auth} AS a ON a.UUID=ua.PrincipalID"
            . " LEFT JOIN {GridUser} AS gu ON gu.UserID=ua.PrincipalID"
            . " WHERE ". implode(' AND ', $queries);

    d4os_io_db_070_set_active('os_robust');
    $results = db_query($query, $values);
    foreach ($results as $result) {
      $users[] = $result;
    }
    d4os_io_db_070_set_active('default');

    if (count($users)) {
      return $users;
    }
    return NULL;
	}

	function save_user($grid_user) {
    if (!isset($grid_user->PrincipalID) || empty($grid_user->PrincipalID) || $grid_user->PrincipalID == UUID_ZERO) {
      return FALSE;
    }
    // check if the user already exists
    d4os_io_db_070_set_active('os_robust');
    $user_exists = db_query("SELECT * FROM {UserAccounts} WHERE PrincipalID=:PrincipalID", array(':PrincipalID' => $grid_user->PrincipalID))->fetchObject();
		d4os_io_db_070_set_active('default');

    // check if another user with the same name exists
    /*d4os_io_db_070_set_active('os_robust');
    $username_exists = db_fetch_object(db_query("SELECT * FROM {UserAccounts} WHERE FirstName='%s' AND LastName='%s'", array($grid_user->FirstName, $grid_user->LastName)));
		d4os_io_db_070_set_active('default');
    if (!$user_exists && is_object($username_exists)) {
      return FALSE;
    }*/

    if ($user_exists) {
			// update the UserAccounts table
      $values = array();
      foreach ($grid_user as $key => $value) {
        switch ($key) {
          //case 'PrincipalID':
          case 'ScopeID':
          case 'FirstName':
          case 'LastName':
          case 'Email':
          case 'Created':
          case 'UserLevel':
            $values[$key] = $value;
            break;
        }
      }
      if (count($values)) {
        // make the query
        d4os_io_db_070_set_active('os_robust');
        $success = db_update('UserAccounts')
          ->fields($values)
          ->condition('PrincipalID', $grid_user->PrincipalID)
          ->execute();
        d4os_io_db_070_set_active('default');
        /*if (!$success) {
          // The query failed - better to abort the save than risk further data loss.
          return FALSE;
        }*/
     }
      // update the auth table
      $values = array();
      foreach ($grid_user as $key => $value) {
        switch ($key) {
          case 'passwordSalt':
          case 'passwordHash':
          case 'webLoginKey':
          case 'accountType':
            $values[$key] = $value;
            break;
        }
      }
      if (count($values)) {
        // make the query
        d4os_io_db_070_set_active('os_robust');
        $success = db_update('auth')
          ->fields($values)
          ->condition('UUID', $grid_user->PrincipalID)
          ->execute();
        d4os_io_db_070_set_active('default');
        /*if (!$success) {
          // The query failed - better to abort the save than risk further data loss.
          return FALSE;
        }*/
      }
			// update the GridUser table
      $values = array();
      foreach ($grid_user as $key => $value) {
        switch ($key) {
          case 'HomeRegionID':
          case 'HomePosition':
          case 'HomeLookAt':
          case 'LastRegionID':
          case 'LastPosition':
          case 'LastLookAt':
          case 'Login':
            $values[$key] = $value;
            break;
        }
      }
      if (count($values)) {
        // make the query
        d4os_io_db_070_set_active('os_robust');
        $success = db_update('GridUser')
          ->fields($values)
          ->condition('UserID', $grid_user->PrincipalID)
          ->execute();
        d4os_io_db_070_set_active('default');
        /*if (!$success) {
          // The query failed - better to abort the save than risk further data loss.
          return FALSE;
        }*/
      }
		}
		else {
      // user does not exist so create a new one

			// insert in the UserAccounts table
      $values   = array();
      // add blank value to "ScopeID"
      $grid_user->ScopeID = UUID_ZERO;
      $grid_user->ServiceURLs = "HomeURI= GatekeeperURI= InventoryServerURI= AssetServerURI=";
      // parse values
      foreach ($grid_user as $key => $value) {
        switch ($key) {
          case 'PrincipalID':
          case 'ScopeID':
          case 'FirstName':
          case 'LastName':
          case 'Email':
          case 'ServiceURLs':
          case 'Created':
          case 'UserLevel':
            $values[$key] = $value;
            break;
        }
      }

      d4os_io_db_070_set_active('os_robust');
      $success = db_insert('UserAccounts')
          ->fields($values)
          ->execute();
      d4os_io_db_070_set_active('default');

      /*if (!$success) {
        // On a failed INSERT some other existing user's uid may be returned.
        // We must abort to avoid overwriting their account.
        return FALSE;
      }*/

    // insert in the auth table
      $values   = array('UUID' => $grid_user->PrincipalID);
      foreach ($grid_user as $key => $value) {
        switch ($key) {
          case 'passwordSalt':
          case 'passwordHash':
          case 'webLoginKey':
          case 'accountType':
            $values[$key] = $value;
            break;
        }
      }

      d4os_io_db_070_set_active('os_robust');
      $success = db_insert('auth')
          ->fields($values)
          ->execute();
      d4os_io_db_070_set_active('default');
      /*if (!$success) {
        // On a failed INSERT some other existing user's uid may be returned.
        // We must abort to avoid overwriting their account.
        return FALSE;
      }*/

    // insert in the GridUser table
      $values   = array('UserID' => $grid_user->PrincipalID);
      foreach ($grid_user as $key => $value) {
        switch ($key) {
          case 'HomeRegionID':
          case 'HomePosition':
          case 'HomeLookAt':
          case 'LastRegionID':
          case 'LastPosition':
          case 'LastLookAt':
          case 'Login':
            $values[$key] = $value;
            break;
        }
      }

      d4os_io_db_070_set_active('os_robust');
      $success = db_insert('GridUser')
          ->fields($values)
          ->execute();
      d4os_io_db_070_set_active('default');
      /*if (!$success) {
        // On a failed INSERT some other existing user's uid may be returned.
        // We must abort to avoid overwriting their account.
        return FALSE;
      }*/

      // create the inventory
      $d4os_inventory = D4OS_IO::create('Inventory');
      if (!isset($grid_user->defaultModel) || $grid_user->defaultModel == UUID_ZERO) {
        $d4os_inventory->create_new_inventory(array('user_uuid' => $grid_user->PrincipalID));
      }
      else {
        $params = array(
          'avatar_src_uuid' => $grid_user->defaultModel,
          'avatar_dest_uuid' => $grid_user->PrincipalID,
        );
        $d4os_inventory->clone_model($params);
      }
		}
    return $this->load_user(array("PrincipalID" => $grid_user->PrincipalID));
  }

	function delete_user($uuid) {
    // delete from UserAccounts
    d4os_io_db_070_set_active('os_robust');
    db_delete('UserAccounts')->condition('PrincipalID', $uuid)->execute();
    d4os_io_db_070_set_active('default');
		// delete from auth
    d4os_io_db_070_set_active('os_robust');
    db_delete('auth')->condition('UUID', $uuid)->execute();
    d4os_io_db_070_set_active('default');
		// delete from GridUser
    d4os_io_db_070_set_active('os_robust');
    db_delete('GridUser')->condition('UserID', $uuid)->execute();
    d4os_io_db_070_set_active('default');
    // delete from Presence
    d4os_io_db_070_set_active('os_robust');
    db_delete('Presence')->condition('UserID', $uuid)->execute();
    d4os_io_db_070_set_active('default');
		// delete from tokens
    d4os_io_db_070_set_active('os_robust');
    db_delete('tokens')->condition('UUID', $uuid)->execute();
    d4os_io_db_070_set_active('default');
    // delete from friends
    d4os_io_db_070_set_active('os_robust');
    db_delete('Friends')->condition('PrincipalID', $uuid)->execute();
    d4os_io_db_070_set_active('default');
    drupal_set_message(t('User deleted from grid.'));

		// delete inventory
		$d4os_inventory = D4OS_IO::create('Inventory');
    $d4os_inventory->delete_user_inventory($uuid);

		return;
	}

  function get_online_users_list($values) {
    $users = new stdClass();
    $users->users = array();
    $where = array();

    // build the main query
    $main_query = " FROM Presence AS p"
                . " LEFT JOIN UserAccounts AS ua ON ua.PrincipalID=p.UserID"
                . " LEFT JOIN regions AS r ON r.uuid=p.RegionID"
                . " LEFT JOIN GridUser AS gu ON gu.UserID=p.UserID";
    if (isset($values['online'])) {
      $where[] = "gu.Online='True'";
    }
    if (isset($values['range']) && is_numeric($values['range'])) {
      $where[] = "gu.Login > ". (time() - $values['range']);
    }
    if (count($where)) {
      $main_query .= ' WHERE '. implode(' AND ', $where);
    }

    // get the quantity of users in Presence table
    $query = "SELECT COUNT(p.UserID)". $main_query;
    d4os_io_db_070_set_active('os_robust');
    $users->count_presence = db_query($query)->fetchField();
    d4os_io_db_070_set_active('default');

    // get the quantity of "online" users in the GridUser table
    $users->count_griduser = $this->get_online_users_count();

    // get users
    $query = "SELECT p.UserID"
           . ", ua.FirstName"
           . ", ua.LastName"
           . ", r.regionName"
           . ", gu.Login"
           . ", gu.Logout"
           . ", gu.LastPosition". $main_query;
    $query .= " ORDER BY :order_by :order_way";
    $options = array(
      ':order_by' => $values['order_by'],
      ':order_way' => $values['order_way']
    );

    d4os_io_db_070_set_active('os_robust');
    $results = db_query_range($query, $values['offset'], $values['limit'], $options)->fetchAll();
    foreach ($results as $result) {
      $users->users[] = $result;
    }
    d4os_io_db_070_set_active('default');

    return $users;
	}

  function get_online_users_count($online = FALSE, $range = 0) {
    // day = 86400 / month = 2419200 / year = 29030400
    $query = "SELECT COUNT(UserID) FROM {GridUser}";
    $where = array();
    if (!$range || !is_numeric($range)) {
      $where[] = "Online='True'";
    }
    else {
      if ($online) {
        $where[] = "Online='True'";
      }
      $where[] = "Login > ". time() - $range;
    }
    if (count($where)) {
      $query .= ' WHERE '. implode(' AND ', $where);
    }

    d4os_io_db_070_set_active('os_robust');
    $result = db_query($query)->fetchField();
    d4os_io_db_070_set_active('default');

    return $result;
  }

  function get_users_count() {
    d4os_io_db_070_set_active('os_robust');
    $count = db_query("SELECT COUNT(PrincipalID) FROM {UserAccounts}")->fetchField();
    d4os_io_db_070_set_active('default');
    return $count;
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
