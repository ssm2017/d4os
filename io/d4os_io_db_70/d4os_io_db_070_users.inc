<?php
/**
 * @package    d4os_io_db
 * @subpackage users
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * Save a user account values on the grid
 * @param Mixed An Array or Object containing drupal user object + an array containing grid data
 * @return Mixed $user object or Boolean FALSE if error
 */
function d4os_io_db_070_os_user_save($account) {
  // convert array to object
  $account = is_array($account) ? (object)$account : $account;

  // get user fields
  $user_fields = d4os_ui_users_get_grid_fields();

  // get all the grid keys
  $grid_user = d4os_ui_users_account_to_grid((array)$account);

  if (!isset($grid_user['username']) && !isset($grid_user['lastname'])) {
    $names = explode(' ', $account->name);
    $grid_user['username'] = $names[0];
    $grid_user['lastname'] = $names[1];
  }

  // separate services
  $fields = array(
    'UserAccounts' => array(
      'table_fields' => array(
        'PrincipalID',
        'ScopeID',
        'FirstName',
        'LastName',
        'Email',
        'ServiceURLs',
        'Created',
        'UserLevel',
        'UserFlags',
        'UserTitle',
      ),
      'allowed_fields' => array(
        'UUID',
        'username',
        'lastname',
        'created',
        'godLevel',
        'email',
      ),
      'submitted_fields' => array(),
      'submitted_values' => array(),
    ),
    'auth' => array(
      'table_fields' => array(
        'UUID',
        'passwordHash',
        'passwordSalt',
        'webLoginKey',
        'accountType',
      ),
      'allowed_fields' => array(
        'passwordSalt',
        'passwordHash',
      ),
      'submitted_fields' => array(),
      'submitted_values' => array(),
    ),
    'GridUser' => array(
      'table_fields' => array(
        'UserID',
        'HomeRegionID',
        'HomePosition',
        'HomeLookAt',
        'LastRegionID',
        'LastLookAt',
        'Online',
        'Login',
        'Logout'
      ),
      'allowed_fields' => array(
        'homeRegion',
        'lastLogin',
      ),
      'submitted_fields' => array(),
      'submitted_values' => array(),
    )
  );

  // UserAccounts items
  foreach ($grid_user as $key => $value) {
    if (in_array($key, $fields['UserAccounts']['allowed_fields'])) {
      switch ($key) {
        case 'UUID':
          $fields['UserAccounts']['submitted_fields'][] = 'PrincipalID';
          $fields['UserAccounts']['submitted_values'][] = $value;
          break;
        case 'username':
          $fields['UserAccounts']['submitted_fields'][] = 'FirstName';
          $fields['UserAccounts']['submitted_values'][] = $value;
          break;
        case 'lastname':
          $fields['UserAccounts']['submitted_fields'][] = 'LastName';
          $fields['UserAccounts']['submitted_values'][] = $value;
          break;
        case 'created':
          $fields['UserAccounts']['submitted_fields'][] = 'Created';
          $fields['UserAccounts']['submitted_values'][] = $value;
          break;
        case 'godLevel':
          $fields['UserAccounts']['submitted_fields'][] = 'UserLevel';
          $fields['UserAccounts']['submitted_values'][] = $value;
          break;
        case 'email':
          $fields['UserAccounts']['submitted_fields'][] = 'Email';
          $fields['UserAccounts']['submitted_values'][] = $value;
          break;
        /*case 'userInventoryURI':
          $fields['UserAccounts']['submitted_fields'][] = '';
          $fields['UserAccounts']['submitted_values'][] = $value;
          break;
        case 'userAssetURI':
          $fields['UserAccounts']['submitted_fields'][] = '';
          $fields['UserAccounts']['submitted_values'][] = $value;
          break;*/
      }
    }
    if (in_array($key, $fields['auth']['allowed_fields'])) {
      switch ($key) {
        case 'passwordSalt':
          $fields['auth']['submitted_fields'][] = 'passwordSalt';
          $fields['auth']['submitted_values'][] = $value;
          break;
        case 'passwordHash':
          $fields['auth']['submitted_fields'][] = 'passwordHash';
          $fields['auth']['submitted_values'][] = $value;
          break;
      }
    }
    if (in_array($key, $fields['GridUser']['allowed_fields'])) {
      switch ($key) {
        case 'homeRegion':
          $fields['GridUser']['submitted_fields'][] = 'HomeRegionID';
          $fields['GridUser']['submitted_values'][] = $value;
          break;
        /*case 'homeLocationX':
          $fields['GridUser']['submitted_fields'][] = '';
          $fields['GridUser']['submitted_values'][] = $value;
          break;
        case 'homeLocationY':
          $fields['GridUser']['submitted_fields'][] = '';
          $fields['GridUser']['submitted_values'][] = $value;
          break;
        case 'homeLocationZ':
          $fields['GridUser']['submitted_fields'][] = '';
          $fields['GridUser']['submitted_values'][] = $value;
          break;
        case 'homeLookAtX':
          $fields['GridUser']['submitted_fields'][] = '';
          $fields['GridUser']['submitted_values'][] = $value;
          break;
        case 'homeLookAtY':
          $fields['GridUser']['submitted_fields'][] = '';
          $fields['GridUser']['submitted_values'][] = $value;
          break;
        case 'homeLookAtZ':
          $fields['GridUser']['submitted_fields'][] = '';
          $fields['GridUser']['submitted_values'][] = $value;
          break;*/
        case 'lastLogin':
          $fields['GridUser']['submitted_fields'][] = 'Login';
          $fields['GridUser']['submitted_values'][] = $value;
          break;
      }
    }
  }

  // check if the user already exists
  $user_exists = d4os_io_db_070_os_user_load(array('UUID' => $grid_user['UUID']));
  if (is_object($user_exists)) {

  // update the UserAccounts table
    $items_count = count($fields['UserAccounts']['submitted_fields']);
    if ($items_count) {
      // parse
      $queries = '';
      $values = array();
      foreach ($fields['UserAccounts']['submitted_fields'] as $key => $value) {
        switch ($value) {
          case 'PrincipalID':
          case 'ScopeID':
          case 'FirstName':
          case 'LastName':
          case 'Email':
            $queries .= "$value = '%s', ";
            $values[] = $fields['UserAccounts']['submitted_values'][$key];
            break;
          case 'Created':
          case 'UserLevel':
            $queries .= "$value = %d, ";
            $values[] = $fields['UserAccounts']['submitted_values'][$key];
            break;
        }
      }

      // remove the last ", "
      $queries = substr($queries, 0, -2);
      // make the query
      d4os_io_db_070_set_active('os_robust');
      $success = db_query("UPDATE {UserAccounts} SET $queries WHERE `PrincipalID` = '%s'", array_merge($values, array($grid_user['UUID'])));
      d4os_io_db_070_set_active('default');
      if (!$success) {
        // The query failed - better to abort the save than risk further data loss.
        return FALSE;
      }
    }

  // update the auth table
    $items_count = count($fields['auth']['submitted_fields']);
    if ($items_count) {
      $queries = '';
      $values = array();
      foreach ($fields['auth']['submitted_fields'] as $key => $value) {
        switch ($value) {
          case 'passwordSalt':
          case 'passwordHash':
            $queries .= "$value = '%s', ";
            $values[] = $fields['auth']['submitted_values'][$key];
            break;
        }
      }

      // remove the last ", "
      $queries = substr($queries, 0, -2);
      // make the query
      d4os_io_db_070_set_active('os_robust');
      $success = db_query("UPDATE {auth} SET $queries WHERE `UUID` = '%s'", array_merge($values, array($grid_user['UUID'])));
      d4os_io_db_070_set_active('default');
      if (!$success) {
        // The query failed - better to abort the save than risk further data loss.
        return FALSE;
      }
    }

  // update the GridUsers table
    $items_count = count($fields['GridUser']['submitted_fields']);
    if ($items_count) {
      $queries = '';
      $values = array();
      foreach ($fields['GridUser']['submitted_fields'] as $key => $value) {
        switch ($value) {
          case 'HomeRegionID':
            $queries .= "$value = '%s', ";
            $values[] = $fields['GridUser']['submitted_values'][$key];
            break;
          case 'Login':
            $queries .= "$value = %d, ";
            $values[] = $fields['UserAccounts']['submitted_values'][$key];
            break;
        }
      }

      // remove the last ", "
      $queries = substr($queries, 0, -2);
      // make the query
      d4os_io_db_070_set_active('os_robust');
      $success = db_query("UPDATE {GridUser} SET $queries WHERE `UserID` = '%s'", array_merge($values, array($grid_user['UUID'])));
      d4os_io_db_070_set_active('default');
      if (!$success) {
        // The query failed - better to abort the save than risk further data loss.
        return FALSE;
      }
    }

    // check if the account is linked
    $uid = db_result(db_query("SELECT count(uid) FROM {d4os_ui_users} WHERE UUID = '%s'", array($grid_user['UUID'])));
    if (!$uid) {
      db_query("INSERT INTO {d4os_ui_users} (uid, UUID) VALUES (%d, '%s')", array($account->uid, $grid_user['UUID']));
    }
  }
  else {
    // create the new user

  // insert in the UserAccounts table
    $items_count = count($fields['UserAccounts']['submitted_fields']);
    if ($items_count) {
      $queries  = array();
      $values   = array();
      $s        = array();
      // add blank value to "ScopeID"
      $fields['UserAccounts']['submitted_fields'][] = 'ScopeID';
      $fields['UserAccounts']['submitted_values'][] = UUID_ZERO;
      // parse values
      foreach ($fields['UserAccounts']['submitted_fields'] as $key => $value) {
        switch ($value) {
          case 'PrincipalID':
          case 'ScopeID':
          case 'FirstName':
          case 'LastName':
          case 'Email':
            $queries[] = $value;
            $values[] = $fields['UserAccounts']['submitted_values'][$key];
            $s[] = "'%s'";
            break;
          case 'Created':
          case 'UserLevel':
            $queries[] = $value;
            $values[] = $fields['UserAccounts']['submitted_values'][$key];
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
    }

  // insert in the auth table
    $items_count = count($fields['auth']['submitted_fields']);
    if ($items_count) {
      $queries  = array('UUID');
      $values   = array($grid_user['UUID']);
      $s        = array("'%s'");
      foreach ($fields['auth']['submitted_fields'] as $key => $value) {
        switch ($value) {
          case 'passwordSalt':
          case 'passwordHash':
            $queries[] = $value;
            $values[] = $fields['auth']['submitted_values'][$key];
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
    }

  // insert in the GridUser table
    $items_count = count($fields['GridUser']['submitted_fields']);
    if ($items_count) {
      $queries  = array('UserID');
      $values   = array($grid_user['UUID']);
      $s        = array("'%s'");
      foreach ($fields['GridUser']['submitted_fields'] as $key => $value) {
        switch ($value) {
          case 'HomeRegionID':
            $queries[] = $value;
            $values[] = $fields['GridUser']['submitted_values'][$key];
            $s[] = "'%s'";
            $queries[] = 'LastRegionID';
            $values[] = $fields['GridUser']['submitted_values'][$key];
            $s[] = "'%s'";
            break;
          case 'Login':
            $queries[] = $value;
            $values[] = $fields['GridUser']['submitted_values'][$key];
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
    }

    db_query("INSERT INTO {d4os_ui_users} (uid, UUID) VALUES (%d, '%s')", array($account->uid, $grid_user['UUID']));
  }

  // Refresh user object.
  $user = d4os_io_db_070_os_user_load($grid_user);

  return $user;
}

/**
 * Load a user object
 * @param Array An array of search filter like array('name'=>'test'); or an integer 
 * @return Mixed $user object or Boolean FALSE if error
 */
function d4os_io_db_070_os_user_load($data = array()) {

  if (is_numeric($data)) {

    // get the user by uid
    $UUID = db_result(db_query("SELECT UUID FROM {d4os_ui_users} WHERE uid = %d", array($data)));

    if (!$UUID) {
      return FALSE;
    }

    $query = "SELECT * FROM {UserAccounts} AS ua"
          . " LEFT JOIN {auth} AS a ON a.UUID=ua.PrincipalID"
          . " LEFT JOIN {GridUser} AS gu ON gu.UserID=ua.PrincipalID"
          . " WHERE ua.PrincipalID='%s'";

    d4os_io_db_070_set_active('os_robust');
    $user = db_fetch_object(db_query($query, $UUID));
    d4os_io_db_070_set_active('default');
    
    if ($user) {
      $user = _d4os_io_db_070_os_070_to_grid($user);
      d4os_io_db_070_users_add_extra_fields($user);
      return $user;
    }
    else {
      return FALSE;
    }
  }

  // get only inworld fields
  $user_fields = d4os_ui_users_get_grid_fields();

  // Dynamically compose a SQL query:
  $query = array();
  $values = array();

  // get the user by keys
  foreach ($data as $key => $value) {
    if (in_array($key, $user_fields)) {
      switch ($key) {
        case 'UUID':
          $query[]= "ua.PrincipalID = '%s'";
          $values[] = $value;
          break;
        case 'username':
          $query[]= "ua.FirstName = '%s'";
          $values[] = $value;
          break;
        case 'lastname':
          $query[]= "ua.LastName = '%s'";
          $values[] = $value;
          break;
        case 'email':
          $query[]= "ua.Email = '%s'";
          $values[] = $value;
          break;
        case 'created':
          $query[]= "ua.Created = %d";
          $values[] = $value;
          break;
        case 'godLevel':
          $query[]= "ua.UserLevel = %d";
          $values[] = $value;
          break;
      }
    }
  }
  $sql = "SELECT * FROM {UserAccounts} AS ua"
          . " LEFT JOIN {auth} AS a ON a.UUID=ua.PrincipalID"
          . " LEFT JOIN {GridUser} AS gu ON gu.UserID=ua.PrincipalID"
          . " WHERE ". implode(' AND ', $query);

  d4os_io_db_070_set_active('os_robust');
  $user = db_fetch_object(db_query($sql, $values));
  d4os_io_db_070_set_active('default');
  
  if ($user) {
    $uid = db_result(db_query("SELECT uid FROM {d4os_ui_users} WHERE UUID = '%s'", array($user->PrincipalID)));
    $user->uid = $uid;
    $user = _d4os_io_db_070_os_070_to_grid($user);
    d4os_io_db_070_users_add_extra_fields($user);
    return $user;
  }
  else {
    return FALSE;
  }
}

/**
 * Add some extra fields to the user object by reference
 * @param Object $user object
 */
function d4os_io_db_070_users_add_extra_fields(&$user) {
  if (module_exists('d4os_io_services_profile')) {
    $properties = d4os_io_db_070_os_profile_services_avatar_properties_request(array('avatar_id' => $user->UUID));
    if (isset($properties['data'][0])) {
      $user->profileURL         = $properties['data'][0]['ProfileUrl'];
      $user->profileImage       = $properties['data'][0]['Image'];
      $user->profileAboutText   = $properties['data'][0]['AboutText'];
      $user->profileFirstImage  = $properties['data'][0]['FirstLifeImage'];
      $user->profileFirstText   = $properties['data'][0]['FirstLifeAboutText'];
      $user->profilePartner     = $properties['data'][0]['Partner'];

      //$user->profileAllowPublish  = $properties['data'][0]['profileAllowPublish'];
      //$user->profileMaturePublish = $properties['data'][0]['profileMaturePublish'];

      // interests
      $user->profileWantDoMask  = $properties['data'][0]['wantmask'];
      $user->profileWantToText  = $properties['data'][0]['wanttext'];
      $user->profileSkillsMask  = $properties['data'][0]['skillsmask'];
      $user->profileSkillsText  = $properties['data'][0]['skillstext'];
      $user->profileLanguages   = $properties['data'][0]['languages'];
    }
  }
}

/**
 * Assign a grid uuid to a Drupal user id
 * @param Integer Drupal user id
 * @param Integer Grid user uuid
 */
function d4os_io_db_070_os_user_set_uid($uid, $uuid) {
  // check if key already exists
  $exists = db_result(db_query("SELECT count(*) FROM {d4os_ui_users} WHERE uid=%d AND UUID='%s'", array($uid, $uuid)));
  if ($exists == 0) {
    db_query("INSERT INTO {d4os_ui_users} (uid, UUID) VALUES (%d, '%s')", array($uid, $uuid));
  }
}

/**
 * Get the uid of a Drupal user by the Grid uuid
 * @param String uuid of the user
 * @return Integer uid corresponding to the uuid
 */
function d4os_io_db_070_os_user_get_uid($uuid) {
  return db_result(db_query("SELECT uid FROM {d4os_ui_users} WHERE UUID = '%s'", array($uuid)));
}

/**
 * Get a list of all the uids mapped to uuids
 * @return Array An array of objects
 */
function d4os_io_db_070_os_user_get_all_uid() {
  $list = array();
  $result = db_query("SELECT * FROM {d4os_ui_users}");
  while ($user = db_fetch_object($result)) {
    $list[] = $user;
  }
  return $list;
}

/**
 * Load all the users of the grid
 * @return Array An array of Obects
 */
function d4os_io_db_070_os_user_load_all() {
  $users = array();
  d4os_io_db_070_set_active('os_robust');
  $result = db_query("SELECT *, ua.FirstName AS username, ua.LastName AS lastname, ua.PrincipalID AS UUID, ua.Email AS email, CONCAT_WS(' ', FirstName, LastName) AS name FROM {UserAccounts} AS ua");
  while ($user = db_fetch_object($result)) {
    $users[] = $user;
  }
  d4os_io_db_070_set_active('default');
  return $users;
}

/**
 * Load all the users of the grid by uuids
 * @param String (optionnal) A where clause begining by a space
 * @return Array An array of Obects
 */
function d4os_io_db_070_os_users_load_by_uuids($uuids) {
  if (!is_array($uuids)) {
    return FALSE;
  }
  $users = array();
  $query = "SELECT *, CONCAT_WS(' ', FirstName, LastName) AS name FROM {UserAccounts} AS ua"
        . " LEFT JOIN {auth} AS a ON a.UUID=ua.PrincipalID"
        . " LEFT JOIN {GridUser} AS gu ON gu.UserID=ua.PrincipalID"
        . " WHERE ua.PrincipalID IN (". implode("','", $uuids). "')";

  d4os_io_db_070_set_active('os_users');
  $result = db_query($query);
  while ($user = db_fetch_object($result)) {
    $users[] = $user;
  }
  d4os_io_db_070_set_active('default');

  return $users;
}

/**
 * Delete a user in the grid and all the mapped items
 * @param String The uuid of the user to delete
 */
function d4os_io_db_070_os_user_delete($uuid) {
  // TODO : delete all assets (impossible ?)
  // TODO : delete groups

  // delete offline messages
  drupal_set_message(t('Deleting offline messages.'));
  d4os_io_db_070_offline_message_user_delete($uuid);
/*
  // delete search
  drupal_set_message(t('Deleting search info.'));
  d4os_io_db_070_os_search_user_delete($uuid);
*/
  // delete profile
  drupal_set_message(t('Deleting profile info.'));
  d4os_io_db_070_os_profile_user_delete($uuid);

  // delete attachments and inventory
  drupal_set_message(t('Deleting inventory.'));
  d4os_io_db_070_os_inventory_user_delete($uuid);

  // delete sessions
  drupal_set_message(t('Deleting sessions.'));
  d4os_io_db_070_set_active('os_robust');
  db_query("DELETE FROM {Presence} WHERE UserID='%s'", $uuid);
  d4os_io_db_070_set_active('default');
/*
  // delete friends
  drupal_set_message(t('Deleting friends.'));
  d4os_io_db_070_set_active('os_users');
  db_query("DELETE FROM {userfriends} WHERE ownerID='%s' OR friendID = '%s'", array($uuid, $uuid));
  d4os_io_db_070_set_active('default');
*/
  // delete user entry in the grid
  drupal_set_message(t('Deleting user.'));
  d4os_io_db_070_set_active('os_robust');
  db_query("DELETE FROM {UserAccounts} WHERE PrincipalID='%s'", $uuid);
  d4os_io_db_070_set_active('default');
  d4os_io_db_070_set_active('os_robust');
  db_query("DELETE FROM {auth} WHERE UUID='%s'", $uuid);
  d4os_io_db_070_set_active('default');
  d4os_io_db_070_set_active('os_robust');
  db_query("DELETE FROM {GridUser} WHERE UserID='%s'", $uuid);
  d4os_io_db_070_set_active('default');

  // delete the user drupal link  
  db_query("DELETE FROM {d4os_ui_users} WHERE UUID='%s'", $uuid);
}

/**
 * Check if the user database is responding
 * @return Array An array like array('success'=>true or false);
 */
function d4os_io_db_070_os_users_is_alive() {
  return array(
    'success' => d4os_io_db_070_mysql_is_alive('os_robust', 'os robust'),
  );
}

/* Grid Monitor tools */

/**
 * Return a paged list of online users
 * @param Integer page number start
 * @param Integer quantity of entries
 * @param Array a Drupal table headers list
 * @return Array An array of objects
 */
function d4os_io_db_070_os_users_online_users_list_paged($page = 0, $limit = 5, $headers) {
  if (!isset($_GET['page'])) {
    $_GET['page'] = $page;
  }
  if (!isset($_GET['order'])) {
    $_GET['order'] = 'loginTime';
  }
  if (!isset($_GET['sort'])) {
    $_GET['sort'] = 'DESC';
  }
  /*
   * uuid
   * agentIP
   * agentPort
   * loginTime
   * logoutTime
   * currentRegion
   * currentRegionName
   * currentHandle
   * currentPos
   * currentLookAt
   */
  d4os_io_db_070_set_active('os_robust');
  $result = pager_query("SELECT * FROM {Presence} AS p"
                        . " LEFT JOIN {UserAccounts} AS ua ON ua.PrincipalID=p.UserID"
                        . " LEFT JOIN {Regions} AS r ON r.uuid=p.RegionID"
                        . " LEFT JOIN {GridUser} AS gu ON gu.UserID=p.UserID"
                        . " WHERE gu.Online = 'true'"
                        . " AND gu.Login < (UNIX_TIMESTAMP(FROM_UNIXTIME(UNIX_TIMESTAMP(now()))))"
                        . " AND gu.Logout < (UNIX_TIMESTAMP(FROM_UNIXTIME(UNIX_TIMESTAMP(now()))))"
                        . " %s", $limit, 0, NULL, array(tablesort_sql($headers)));
  while ($user = db_fetch_object($result)) {
    $items[] = $user;
  }
  d4os_io_db_070_set_active('default');
  return $items;
}

/**
 * Return the online users list
 * @return Integer quantity of users actually online
 */
function d4os_io_db_070_os_users_online_users_count() {
  d4os_io_db_070_set_active('os_robust');
  $now_online = db_result(db_query("SELECT COUNT(*) FROM {GridUser}"
                        . " WHERE Online = 'true'"
                        . " AND Login < (UNIX_TIMESTAMP(FROM_UNIXTIME(UNIX_TIMESTAMP(now()))))"
                        . " AND Logout < (UNIX_TIMESTAMP(FROM_UNIXTIME(UNIX_TIMESTAMP(now()))))"));
  d4os_io_db_070_set_active('default');
  return $now_online;
}

/**
 * Transforms the database fields in d4os account
 * @todo : this has to be changed to something else even for 69
 */
function _d4os_io_db_070_os_070_to_grid($params) {
  $user = new stdClass;
  //$params->ScopeID;
  //$params->UserFlags;
  //$params->UserTitle;
  //$params->UUID;
  //$params->webLoginKey;
  //$params->accountType;
  //$params->UserID;
  //$params->RegionID;
  //$params->SessionID;
  //$params->SecureSessionID;
  //$params->Online;
  //$params->Logout;
  //$params->LookAt;

  $user->UUID                 = $params->PrincipalID;
  $user->username             = $params->FirstName;
  $user->lastname             = $params->LastName;
  $user->userInventoryURI     = $params->ServiceURLs;
  $user->userAssetURI         = $params->ServiceURLs;
  $user->created              = $params->Created;
  $user->godLevel             = $params->UserLevel;
  $user->email                = $params->Email;

  // auth
  $user->passwordSalt         = $params->passwordSalt;
  $user->passwordHash         = $params->passwordHash;
  $user->md5hash              = NULL;
  $user->a1hash               = NULL;

  // GridUser
  $user->homeRegion           = $params->HomeRegionID;
  preg_match_all('/\<(.*), (.*), (.*)\>/', $params->HomePosition, $temp);
  $user->homeLocationX        = $temp[1][0];
  $user->homeLocationY        = $temp[2][0];
  $user->homeLocationZ        = $temp[3][0];
  preg_match_all('/\<(.*), (.*), (.*)\>/', $params->HomeLookAt, $temp);
  $user->homeLookAtX          = $temp[1][0];
  $user->homeLookAtY          = $temp[2][0];
  $user->homeLookAtZ          = $temp[3][0];
  $user->lastLogin            = $params->Login;

  // profile
  $user->profileCanDoMask     = NULL;
  $user->profileWantDoMask    = NULL;
  $user->profileAboutText     = NULL;
  $user->profileFirstText     = NULL;
  $user->profileImage         = NULL;
  $user->profileFirstImage    = NULL;
  $user->profileURL           = NULL;
  $user->profilePartner       = NULL;

  return $user;
}
