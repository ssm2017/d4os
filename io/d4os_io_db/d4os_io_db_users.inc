<?php
/**
* @package		d4os
* @copyright	Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
* @license		GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
* D4os is free software. This version may have been modified pursuant
* to the GNU General Public License, and as distributed it includes or
* is derivative of works licensed under the GNU General Public License or
* other free or open source software licenses.
*/

function d4os_io_db_os_user_save($account) {

  // convert array to object
  $account = is_array($account) ? (object)$account : $account;

  // get user fields
  $user_fields = d4os_ui_users_get_inworld_fields();

  // get all the inworld keys
  $inworld_user = d4os_ui_users_account_to_inworld_array($account);
  if (!isset($inworld_user['username']) && !isset($inworld_user['lastname'])) {
    $names = explode(' ', $account->name);
    $inworld_user['username'] = $names[0];
    $inworld_user['lastname'] = $names[1];
  }

  // check if the user already exist
  $user_exists = d4os_io_db_os_user_load(array('UUID' => $inworld_user['UUID']));

  if (is_object($user_exists)) {
    $query = '';
    foreach ($inworld_user as $key => $value) {
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
    db_set_active('os_users');
    $success = db_query("UPDATE {users} SET $query WHERE `UUID` = '%s'", array_merge($v, array($inworld_user['UUID'])));
    db_set_active('default');
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
    foreach ($inworld_user as $key => $value) {
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

    db_set_active('os_users');
    $success = db_query('INSERT INTO {users} ('. implode(', ', $fields) .') VALUES ('. implode(', ', $s) .')', $values);
    db_set_active('default');
    if (!$success) {
      // On a failed INSERT some other existing user's uid may be returned.
      // We must abort to avoid overwriting their account.
      return FALSE;
    }
    else {
      db_query("INSERT INTO {d4os_ui_users} (uid, UUID) VALUES (%d, '%s')", array($account->uid, $inworld_user['UUID']));
    }
  }

  // Refresh user object.
  $user = d4os_io_db_os_user_load($inworld_user);

  return $user;
}

function d4os_io_db_os_user_load($array = array()) {
  // get only inworld fields
  $user_fields = d4os_ui_users_get_inworld_fields();

  // Dynamically compose a SQL query:
  $query = array();
  $params = array();

  if (!is_array($array)) {
    return FALSE;
  }

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
  db_set_active('os_users');
  $result = db_query('SELECT * FROM {users} WHERE '. implode(' AND ', $query), $params);
  if ($user = db_fetch_object($result)) {
    db_set_active('default');
    return $user;
  }
  else {
    db_set_active('default');
    return FALSE;
  }
}

function d4os_io_db_os_user_load_all() {
  $users = array();
  db_set_active('os_users');
  $result = db_query('SELECT *, CONCAT_WS(\' \', username, lastname) AS name FROM {users}');
  while ($user = db_fetch_object($result)) {
    $users[] = $user;
  }
  db_set_active('default');
  return $users;
}

function d4os_io_db_os_user_delete($data) {
  // TODO : delete all assets
  // TODO : delete all sessions
  // TODO : delete inventory
  // TODO : delete friends
  db_set_active('os_users');
  $user = db_query("DELETE FROM {users} WHERE UUID='%s'", $data);
  db_set_active('default');
  db_query("DELETE FROM {d4os_ui_users} WHERE UUID='%s'", $data);
}
