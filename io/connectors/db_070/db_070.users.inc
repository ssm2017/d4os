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

	function __construct() {
		$this->values = array(

		);
	}

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
            $queries[]= "ua.PrincipalID = '%s'";
            $values[] = $value;
            break;
          case 'ScopeID':
            $queries[]= "ua.ScopeID = '%s'";
            $values[] = $value;
            break;
          case 'FirstName':
            $queries[]= "ua.FirstName = '%s'";
            $values[] = $value;
            break;
          case 'LastName':
            $queries[]= "ua.LastName = '%s'";
            $values[] = $value;
            break;
          case 'Email':
            $queries[]= "ua.Email = '%s'";
            $values[] = $value;
            break;
          case 'ServiceURLs':
            $queries[]= "ua.ServiceURLs = '%s'";
            $values[] = $value;
            break;
          case 'Created':
            $queries[]= "ua.Created = %d";
            $values[] = $value;
            break;
          case 'UserLevel':
            $queries[]= "ua.UserLevel = %d";
            $values[] = $value;
            break;
          case 'UserTitle':
            $queries[]= "ua.UserTitle = %d";
            $values[] = $value;
            break;
          case 'name':
            $names = explode(' ', $value);
            $queries[]= "ua.FirstName = '%s'";
            $values[] = $names[0];
            $queries[]= "ua.LastName = '%s'";
            $values[] = $names[1];
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
    $result = db_query($query, $values);
    while ($user = db_fetch_object($result)) {
      $users[] = $user;
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
    $user_exists = db_fetch_object(db_query("SELECT * FROM {UserAccounts} WHERE PrincipalID='%s'", $grid_user->PrincipalID));
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
      $queries = '';
      $values = array();
      foreach ($grid_user as $key => $value) {
        switch ($key) {
          //case 'PrincipalID':
          case 'ScopeID':
          case 'FirstName':
          case 'LastName':
          case 'Email':
          case 'UserTitle':
            $queries .= "$key = '%s', ";
            $values[] = $value;
            break;
          case 'Created':
          case 'UserLevel':
            $queries .= "$key = %d, ";
            $values[] = $value;
            break;
        }
      }
      if (!empty($queries)) {
        // remove the last ", "
        $queries = substr($queries, 0, -2);
        // make the query
        d4os_io_db_070_set_active('os_robust');
        $success = db_query("UPDATE {UserAccounts} SET $queries WHERE `PrincipalID` = '%s'", array_merge($values, array($grid_user->PrincipalID)));
        d4os_io_db_070_set_active('default');
        if (!$success) {
          // The query failed - better to abort the save than risk further data loss.
          return FALSE;
        }
     }
      // update the auth table
      $queries = '';
      $values = array();
      foreach ($grid_user as $key => $value) {
        switch ($key) {
          case 'passwordSalt':
          case 'passwordHash':
          case 'webLoginKey':
          case 'accountType':
            $queries .= "$key = '%s', ";
            $values[] = $value;
            break;
        }
      }
      if (!empty($queries)) {
        // remove the last ", "
        $queries = substr($queries, 0, -2);
        // make the query
        d4os_io_db_070_set_active('os_robust');
        $success = db_query("UPDATE {auth} SET $queries WHERE `UUID` = '%s'", array_merge($values, array($grid_user->PrincipalID)));
        d4os_io_db_070_set_active('default');
        if (!$success) {
          // The query failed - better to abort the save than risk further data loss.
          return FALSE;
        }
      }
			// update the GridUser table
      $queries = '';
      $values = array();
      foreach ($grid_user as $key => $value) {
        switch ($key) {
          case 'HomeRegionID':
          case 'HomePosition':
          case 'HomeLookAt':
          case 'LastRegionID':
          case 'LastPosition':
          case 'LastLookAt':
            $queries .= "$key = '%s', ";
            $values[] = $value;
            break;
          case 'Login':
            $queries .= "$key = %d, ";
            $values[] = $value;
            break;
        }
      }
      if (!empty($queries)) {
        // remove the last ", "
        $queries = substr($queries, 0, -2);
        // make the query
        d4os_io_db_070_set_active('os_robust');
        $success = db_query("UPDATE {GridUser} SET $queries WHERE `UserID` = '%s'", array_merge($values, array($grid_user->PrincipalID)));
        d4os_io_db_070_set_active('default');
        if (!$success) {
          // The query failed - better to abort the save than risk further data loss.
          return FALSE;
        }
      }
		}
		else {
      // user does not exist so create a new one

			// insert in the UserAccounts table
      $queries  = array();
      $values   = array();
      $s        = array();
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
          case 'UserTitle':
            $queries[] = $key;
            $values[] = $value;
            $s[] = "'%s'";
            break;
          case 'Created':
          case 'UserLevel':
            $queries[] = $key;
            $values[] = $value;
            $s[] = "%d";
            break;
        }
      }

      d4os_io_db_070_set_active('os_robust');
      $success = db_query('INSERT INTO {UserAccounts} ('. implode(', ', $queries) .') VALUES ('. implode(', ', $s) .')', $values);
      d4os_io_db_070_set_active('default');
      if (!$success) {
        // On a failed INSERT some other existing user's uid may be returned.
        // We must abort to avoid overwriting their account.
        return FALSE;
      }

    // insert in the auth table
      $queries  = array('UUID');
      $values   = array($grid_user->PrincipalID);
      $s        = array("'%s'");
      foreach ($grid_user as $key => $value) {
        switch ($key) {
          case 'passwordSalt':
          case 'passwordHash':
          case 'webLoginKey':
          case 'accountType':
            $queries[] = $key;
            $values[] = $value;
            $s[] = "'%s'";
            break;
        }
      }

      d4os_io_db_070_set_active('os_robust');
      $success = db_query('INSERT INTO {auth} ('. implode(', ', $queries) .') VALUES ('. implode(', ', $s) .')', $values);
      d4os_io_db_070_set_active('default');
      if (!$success) {
        // On a failed INSERT some other existing user's uid may be returned.
        // We must abort to avoid overwriting their account.
        return FALSE;
      }

    // insert in the GridUser table
      $queries  = array('UserID');
      $values   = array($grid_user->PrincipalID);
      $s        = array("'%s'");
      foreach ($grid_user as $key => $value) {
        switch ($key) {
          case 'HomeRegionID':
          case 'HomePosition':
          case 'HomeLookAt':
          case 'LastRegionID':
          case 'LastPosition':
          case 'LastLookAt':
            $queries[] = $key;
            $values[] = $value;
            $s[] = "'%s'";
            break;
          case 'Login':
            $queries[] = $key;
            $values[] = $value;
            $s[] = "%d";
            break;
        }
      }

      d4os_io_db_070_set_active('os_robust');
      $success = db_query('INSERT INTO {GridUser} ('. implode(', ', $queries) .') VALUES ('. implode(', ', $s) .')', $values);
      d4os_io_db_070_set_active('default');
      if (!$success) {
        // On a failed INSERT some other existing user's uid may be returned.
        // We must abort to avoid overwriting their account.
        return FALSE;
      }

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
    db_query("DELETE FROM {UserAccounts} WHERE PrincipalID='%s'", $uuid);
    d4os_io_db_070_set_active('default');
		// delete from auth
    d4os_io_db_070_set_active('os_robust');
    db_query("DELETE FROM {auth} WHERE UUID='%s'", $uuid);
    d4os_io_db_070_set_active('default');
		// delete from GridUser
    d4os_io_db_070_set_active('os_robust');
    db_query("DELETE FROM {GridUser} WHERE UserID='%s'", $uuid);
    d4os_io_db_070_set_active('default');
    // delete from Presence
    d4os_io_db_070_set_active('os_robust');
    db_query("DELETE FROM {Presence} WHERE UserID='%s'", $uuid);
    d4os_io_db_070_set_active('default');
		// delete from tokens
    d4os_io_db_070_set_active('os_robust');
    db_query("DELETE FROM {tokens} WHERE UUID='%s'", $uuid);
    d4os_io_db_070_set_active('default');
    // delete from friends
    d4os_io_db_070_set_active('os_robust');
    db_query("DELETE FROM {Friends} WHERE PrincipalID='%s' OR Friend='%s'", $uuid, $uuid);
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
    $users->count_presence = db_result(db_query($query));
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
    $query .= " ORDER BY %s %s";
    $query .= " LIMIT %d,%d";

    d4os_io_db_070_set_active('os_robust');
    $result = db_query($query, array($values['order_by'], $values['order_way'], $values['offset'], $values['limit']));
    while ($user = db_fetch_object($result)) {
      $users->users[] = $user;
    }
    d4os_io_db_070_set_active('default');

    return $users;
	}

  function get_online_users_count($range = 0) {
    // day = 86400 / month = 2419200 / year = 29030400
    $query = "SELECT COUNT(UserID) FROM {GridUser}";
    $where = array();
    if (!$range || !is_numeric($range)) {
      // Count the true number of users online.  GridUser.Online is not reliable.
      $query = "SELECT COUNT(*) FROM {Presence}";
      // Don't count ghosts.  Sometimes OpenSim gets confused and leaves people online when they are not, but at least sets the region they are in to the NULL key.
      $where[] = "RegionID<>'00000000-0000-0000-0000-000000000000'";
    }
    else {
      $where[] = "Login > UNIX_TIMESTAMP(FROM_UNIXTIME(UNIX_TIMESTAMP(now()) - $range))";
    }
    if (count($where)) {
      $query .= ' WHERE '. implode(' AND ', $where);
    }

    d4os_io_db_070_set_active('os_robust');
    $result = db_result(db_query($query));
    d4os_io_db_070_set_active('default');

    return $result;
  }

  function get_users_count() {
    d4os_io_db_070_set_active('os_robust');
    $count = db_result(db_query("SELECT COUNT(PrincipalID) FROM {UserAccounts}"));
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
    $uid_exists = db_result(db_query("SELECT count(uid) FROM {d4os_ui_users} WHERE UUID = '%s' OR uid = %d", array($uuid, $uid)));
    if ($uid_exists) {
      db_query("DELETE FROM {d4os_ui_users} WHERE UUID = '%s' OR uid = %d", array($uuid, $uid));
    }
    db_query("INSERT INTO {d4os_ui_users} (UUID, uid) VALUES ('%s', %d)", array($uuid, $uid));
  }

}
