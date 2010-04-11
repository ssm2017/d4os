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
  $grid_user = d4os_ui_users_account_to_grid((array)$account);

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
    else {
      // check if the account is linked
      $uid = db_result(db_query("SELECT uid FROM {d4os_ui_users} WHERE UUID = '%s'", array($grid_user['UUID'])));
      if (!$uid) {
        db_query("INSERT INTO {d4os_ui_users} (uid, UUID) VALUES (%d, '%s')", array($account->uid, $grid_user['UUID']));
      }
    }
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

function d4os_io_db_os_user_load($data = array()) {

  if (is_numeric($data)) {
    // get the user by uid
    $UUID = db_result(db_query("SELECT UUID FROM {d4os_ui_users} WHERE uid = %d", array($data)));
    if (!$UUID) {
      return FALSE;
    }
    d4os_io_db_set_active('os_users');
    $result = db_query("SELECT * FROM {users} WHERE UUID = '%s'", array($UUID));
    if ($user = db_fetch_object($result)) {
      d4os_io_db_set_active('default');
      d4os_io_db_users_add_extra_fields($user);
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
  foreach ($data as $key => $value) {
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
    d4os_io_db_users_add_extra_fields($user);
    return $user;
  }
  else {
    d4os_io_db_set_active('default');
    return FALSE;
  }
}

function d4os_io_db_users_add_extra_fields(&$user) {
  if (module_exists('d4os_io_services_profile')) {
    $params = array();
    $params['avatar_id'] = $user->UUID;
    $properties = d4os_io_db_os_profile_services_avatar_properties_request($params);
    if (isset($properties['data'][0])) {
      $user->profilePartner        = $properties['data'][0]['profilePartner']; // 1
      $user->profileAllowPublish   = $properties['data'][0]['profileAllowPublish']; // 2
      $user->profileMaturePublish  = $properties['data'][0]['profileMaturePublish']; // 3
      $user->profileURL            = $properties['data'][0]['profileURL']; // 4
      $user->ProfileUrl            = $properties['data'][0]['profileURL']; // 5
      $user->profileWantToMask     = $properties['data'][0]['profileWantToMask']; // 6
      $user->profileWantToText     = $properties['data'][0]['profileWantToText']; // 7
      $user->profileSkillsMask     = $properties['data'][0]['profileSkillsMask']; // 8
      $user->profileSkillsText     = $properties['data'][0]['profileSkillsText']; // 9
      $user->profileLanguages      = $properties['data'][0]['profileLanguages']; // 10
      $user->profileFirstImage     = $properties['data'][0]['profileFirstImage']; // 11
      $user->profileFirstText      = $properties['data'][0]['profileFirstText']; // 12
    }
  }
}

function d4os_io_db_os_user_set_uid($uid, $uuid) {
  db_query("INSERT INTO {d4os_ui_users} (uid, UUID) VALUES (%d, '%s')", array($uid, $uuid));
}

function d4os_io_db_os_user_get_uid($uuid) {
  return db_result(db_query("SELECT uid FROM {d4os_ui_users} WHERE UUID = '%s'", array($uuid)));
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

function d4os_io_db_os_user_delete($uuid) {
  // TODO : delete all assets (impossible ?)
  // TODO : delete groups

  // delete offline messages
  drupal_set_message(t('Deleting offline messages.'));
  d4os_io_db_offline_message_user_delete($uuid);

  // delete search
  drupal_set_message(t('Deleting search info.'));
  d4os_io_db_os_search_user_delete($uuid);

  // delete profile
  drupal_set_message(t('Deleting profile info.'));
  d4os_io_db_os_profile_user_delete($uuid);

  // delete attachments and inventory
  drupal_set_message(t('Deleting inventory.'));
  d4os_io_db_os_inventory_user_delete($uuid);

  // delete sessions
  drupal_set_message(t('Deleting sessions.'));
  d4os_io_db_set_active('os_users');
  db_query("DELETE FROM {agents} WHERE UUID='%s'", $uuid);
  d4os_io_db_set_active('default');

  // delete friends
  drupal_set_message(t('Deleting friends.'));
  d4os_io_db_set_active('os_users');
  db_query("DELETE FROM {userfriends} WHERE ownerID='%s' OR friendID = '%s'", array($uuid, $uuid));
  d4os_io_db_set_active('default');

  // delete user entry in the grid
  drupal_set_message(t('Deleting user.'));
  d4os_io_db_set_active('os_users');
  db_query("DELETE FROM {users} WHERE UUID='%s'", $uuid);
  d4os_io_db_set_active('default');

  // delete the user drupal link  
  db_query("DELETE FROM {d4os_ui_users} WHERE UUID='%s'", $uuid);
}
