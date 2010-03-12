<?php
/**
* @package    d4os
* @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
* @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
* D4os is free software. This version may have been modified pursuant
* to the GNU General Public License, and as distributed it includes or
* is derivative of works licensed under the GNU General Public License or
* other free or open source software licenses.
*/

function d4os_io_db_os_user_save($account) {
  // convert array to object
  $account = is_array($account) ? (object)$account : $account;

  // get user fields
  $user_fields = d4os_ui_users_get_grid_fields();

  // get all the grid keys
  $grid_user = (array)d4os_ui_users_account_to_grid($account);
  if (!isset($grid_user['username']) && !isset($grid_user['lastname'])) {
    $names = explode(' ', $account->name);
    $grid_user['username'] = $names[0];
    $grid_user['lastname'] = $names[1];
  }

  // check if the user already exists
  $user_exists = d4os_io_db_os_user_load(array('UUID' => $grid_user['UUID']));
  if (is_object($user_exists)) {
    $query = '';
    foreach ($grid_user as $key => $value) {
      if (in_array($key, $user_fields)) {
        switch ($key) {
          //case 'UUID':
          case 'username':
          case 'lastname':
          case 'passwordSalt':
          case 'passwordHash':
          case 'userInventoryURI':
          case 'userAssetURI':
          case 'profileAboutText':
          case 'profileFirstText':
          case 'profileImage':
          case 'profileFirstImage':
          case 'email':
            $query .= "$key = '%s', ";
            $v[] = $value;
            break;
          case 'homeRegion':
          case 'created':
          case 'lastLogin':
          case 'profileCanDoMask':
          case 'profileWantDoMask':
          case 'godLevel':
            $query .= "$key = %d, ";
            $v[] = $value;
            break;
          case 'homeLocationX':
          case 'homeLocationY':
          case 'homeLocationZ':
          case 'homeLookAtX':
          case 'homeLookAtY':
          case 'homeLookAtZ':
            $query .= "$key = %f, ";
            $v[] = $value;
            break;
        }
      }
    }

    // remove the last ", "
    $query = substr($query, 0, -2);
    // make the query
    d4os_io_db_set_active('os_users');
    $success = db_query("UPDATE {users} SET $query WHERE `UUID` = '%s'", array_merge($v, array($grid_user['UUID'])));
    d4os_io_db_set_active('default');
    if (!$success) {
      // The query failed - better to abort the save than risk further data loss.
      return FALSE;
    }

    // TODO : Delete a blocked user's sessions to kick them if they are online.
    //if (isset($array['status']) && $array['status'] == 0) {
    //  sess_destroy_uid($account->uid);
    //}

  }
  else {
    // create the new user
    foreach ($grid_user as $key => $value) {
      if (in_array($key, $user_fields)) {
        switch ($key) {
          case 'UUID':
          case 'username':
          case 'lastname':
          case 'passwordSalt':
          case 'passwordHash':
          case 'userInventoryURI':
          case 'userAssetURI':
          case 'profileAboutText':
          case 'profileFirstText':
          case 'profileImage':
          case 'profileFirstImage':
          case 'email':
            $fields[] = $key;
            $values[] = $value;
            $s[] = "'%s'";
            break;
          case 'homeRegion':
          case 'created':
          case 'lastLogin':
          case 'profileCanDoMask':
          case 'profileWantDoMask':
          case 'godLevel':
            $fields[] = $key;
            $values[] = $value;
            $s[] = "%d";
            break;
          case 'homeLocationX':
          case 'homeLocationY':
          case 'homeLocationZ':
          case 'homeLookAtX':
          case 'homeLookAtY':
          case 'homeLookAtZ':
            $fields[] = $key;
            $values[] = $value;
            $s[] = "%f";
            break;
        }
      }
    }

    d4os_io_db_set_active('os_users');
    $success = db_query('INSERT INTO {users} ('. implode(', ', $fields) .') VALUES ('. implode(', ', $s) .')', $values);
    d4os_io_db_set_active('default');
    if (!$success) {
      // On a failed INSERT some other existing user's uid may be returned.
      // We must abort to avoid overwriting their account.
      return FALSE;
    }
    else {
      db_query("INSERT INTO {d4os_ui_users} (uid, UUID) VALUES (%d, '%s')", array($account->uid, $grid_user['UUID']));
    }
  }

  // Refresh user object.
  $user = d4os_io_db_os_user_load($grid_user);

  return $user;
}

function d4os_io_db_os_user_load($array = array()) {
  if (is_numeric($array)) {
    // get the user by uid
    $UUID = db_result(db_query("SELECT UUID FROM {d4os_ui_users} WHERE uid = %d", array($array)));
    if (!$UUID) {
      return FALSE;
    }
    d4os_io_db_set_active('os_users');
    $result = db_query("SELECT * FROM {users} WHERE UUID = '%s'", array($UUID));
    if ($user = db_fetch_object($result)) {
      d4os_io_db_set_active('default');
      return $user;
    }
    else {
      d4os_io_db_set_active('default');
      return FALSE;
    }
  }

  // get only inworld fields
  $user_fields = d4os_ui_users_get_grid_fields();

  // Dynamically compose a SQL query:
  $query = array();
  $params = array();

  // get the user by keys
  foreach ($array as $key => $value) {
    if (in_array($key, $user_fields)) {
      switch ($key) {
        case 'UUID':
        case 'username':
        case 'lastname':
        case 'passwordSalt':
        case 'passwordHash':
        case 'userInventoryURI':
        case 'userAssetURI':
        case 'profileAboutText':
        case 'profileFirstText':
        case 'profileImage':
        case 'profileFirstImage':
        case 'email':
          $query[]= "$key = '%s'";
          $params[] = $value;
          break;
        case 'homeRegion':
        case 'created':
        case 'lastLogin':
        case 'profileCanDoMask':
        case 'profileWantDoMask':
        case 'godLevel':
          $query[]= "$key = %d";
          $params[] = $value;
          break;
        case 'homeLocationX':
        case 'homeLocationY':
        case 'homeLocationZ':
        case 'homeLookAtX':
        case 'homeLookAtY':
        case 'homeLookAtZ':
          $query[]= "$key = %f";
          $params[] = $value;
          break;
      }
    }
  }
  d4os_io_db_set_active('os_users');
  $result = db_query('SELECT * FROM {users} WHERE '. implode(' AND ', $query), $params);
  if ($user = db_fetch_object($result)) {
    d4os_io_db_set_active('default');
    $uid = db_result(db_query("SELECT uid FROM {d4os_ui_users} WHERE UUID = '%s'", array($user->UUID)));
    $user->uid = $uid;
    return $user;
  }
  else {
    d4os_io_db_set_active('default');
    return FALSE;
  }
}

function d4os_io_db_os_user_load_all() {
  $users = array();
  d4os_io_db_set_active('os_users');
  $result = db_query('SELECT *, CONCAT_WS(\' \', username, lastname) AS name FROM {users}');
  while ($user = db_fetch_object($result)) {
    $users[] = $user;
  }
  d4os_io_db_set_active('default');
  return $users;
}

function d4os_io_db_os_user_delete($data) {
  // TODO : delete all assets
  // TODO : delete all sessions
  // TODO : delete inventory
  // TODO : delete friends
  d4os_io_db_set_active('os_users');
  $user = db_query("DELETE FROM {users} WHERE UUID='%s'", $data);
  d4os_io_db_set_active('default');
  db_query("DELETE FROM {d4os_ui_users} WHERE UUID='%s'", $data);
}
