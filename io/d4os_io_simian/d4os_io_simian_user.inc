<?php
/**
 * @package    d4os_io_simian
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * Save a user account
 */
function d4os_io_simian_os_user_save($account) {
  global $simian_url;

  // convert array to object
  $account = is_array($account) ? (object)$account : $account;
  // get all the grid keys
  $grid_user = d4os_ui_users_account_to_grid((array)$account);
  // check if the user already exists
  $user_exists = d4os_io_simian_os_user_load(array('UUID' => $grid_user['UUID']));

  if (!is_object($user_exists)) {
    db_query("INSERT INTO {d4os_ui_users} (uid, UUID) VALUES (%d, '%s')", array($account->uid, $grid_user['UUID']));
  }

  // add the user
  $params = array(
    'UserID'      => $grid_user['UUID'],
    'Name'        => $account->name,
    'Email'       => $account->mail,
    'AccessLevel' => $grid_user['godLevel'],
  );
  if (_d4os_io_simian_add_user($params)) {
    $params = array(
      'RequestMethod' => 'AddIdentity',
      'Identifier'    => $account->name,
      'Credential'    => $grid_user['md5hash'],
      'Type'          => 'md5hash',
      'UserID'        => $grid_user['UUID'],
    );
    if (_d4os_io_simian_add_identity($params)) {
      $params = array(
        'RequestMethod' => 'AddIdentity',
        'Identifier'    => $account->name,
        'Credential'    => $grid_user['a1hash'],
        'Type'          => 'a1hash',
        'UserID'        => $grid_user['UUID'],
      );
      if (_d4os_io_simian_add_identity($params)) {
        $params = array(
          'OwnerID' => $grid_user['UUID']
        );
        if ($response = _d4os_io_simian_add_inventory($params)) {
          return $grid_user;
        }
        else {
          drupal_set_message(t('Error when adding user inventory to the grid.'), 'error');
          drupal_set_message($response, 'error');
          return FALSE;
        }
      }
      else {
        drupal_set_message(t('Error when adding user webdav password to the grid.'), 'error');
        return FALSE;
      }
    }
    else {
      drupal_set_message(t('Error when adding user password to the grid.'), 'error');
      return FALSE;
    }
  }
  else {
    drupal_set_message(t('Error when adding user to the grid.'), 'error');
    return FALSE;
  }
}

function d4os_io_simian_os_user_load($data = array()) {
  global $simian_url;

  if (is_numeric($data)) {
    // get the user by uid
    $uuid = db_result(db_query("SELECT UUID FROM {d4os_ui_users} WHERE uid = %d", array($data)));
    if (!$uuid) {
      return FALSE;
    }

    $values = drupal_to_js(
      array(
        'RequestMethod' => 'GetUser',
        'UserID'        => $uuid,
      )
    );

    $http_answer = d4os_http_request($simian_url, array('Content-Type' => 'application/json'), 'POST', $values);
    if ($http_answer->code == 200) {
      $result = json_decode($http_answer->data);
      if ($result->Success == 'true') {
        $result->User->uid = $data;
        return _d4os_io_simian_user_simian_to_grid($result->User);
      }
      else {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }

  $params = array(
    'RequestMethod' => 'GetUser',
  );
  if (isset($data['UUID'])) {
    $params['UserID'] = $data['UUID'];
  }
  if (isset($data['name'])) {
    $params['Name'] = $data['name'];
  }
  if (isset($data['username']) && isset($data['lastname'])) {
    $params['Name'] = $data['username']. ' '. $data['lastname'];
  }
  if (isset($data['email'])) {
    $params['Email'] = $data['email'];
  }
  $values = drupal_to_js($params);

  $http_answer = d4os_http_request($simian_url, array('Content-Type' => 'application/json'), 'POST', $values);

  if ($http_answer->code == 200) {
    $result = json_decode($http_answer->data);
    if ($result->Success == 'true') {
      // get the uid
      $uid = db_result(db_query("SELECT uid FROM {d4os_ui_users} WHERE UUID = '%s'", array($result->user['UserID'])));
      $result->User->uid = $uid;
      return _d4os_io_simian_user_simian_to_grid($result->User);
    }
    else {
      return FALSE;
    }
  }
  else {
    return FALSE;
  }
}

function d4os_io_simian_os_user_set_uid($uid, $uuid) {
  db_query("INSERT INTO {d4os_ui_users} (uid, UUID) VALUES (%d, '%s')", array($uid, $uuid));
}

function d4os_io_simian_os_user_get_uid($uuid) {
  return db_result(db_query("SELECT uid FROM {d4os_ui_users} WHERE UUID = '%s'", array($uuid)));
}

function d4os_io_simian_os_user_load_all() {
  
}

function d4os_io_simian_os_user_delete($uuid) {
  // get the user
  $grid_user = d4os_io_simian_os_user_load(array('UUID' => $uuid));

  // delete the user in the grid
  $params = array(
    'UserID' => $uuid
  );
  _d4os_io_simian_remove_user($params);

  // delete the grid password
  $params = array(
    'RequestMethod' => 'RemoveIdentity',
    'Identifier'    => $grid_user->name,
    'Type'          => 'md5hash',
  );
  _d4os_io_simian_remove_identity($params);

  // delete the webdav password
  $params = array(
    'RequestMethod' => 'RemoveIdentity',
    'Identifier'    => $grid_user->name,
    'Type'          => 'a1hash',
  );
  _d4os_io_simian_remove_identity($params);

  // delete the user drupal link  
  db_query("DELETE FROM {d4os_ui_users} WHERE UUID='%s'", $uuid);
}

function _d4os_io_simian_user_simian_to_grid($params) {
  $user = new stdClass;
  $user->UUID       = $params->UserID;
  $names = explode(' ', $params->Name);
  $user->username   = $names[0];
  $user->lastname   = $names[1];
  $user->mail       = $params->Email;
  $user->godLevel   = $params->AccessLevel;
  $user->created    = $params->CreationDate;
  $user->lastLogin  = $params->LastLoginDate;
  $user->name       = $params->Name;
  $user->uid        = $params->uid;
  return $user;
}

function _d4os_io_simian_add_user($params) {
  global $simian_url;
  $values = drupal_to_js(
    array(
      'RequestMethod' => 'AddUser',
      'UserID'        => $params['UserID'],
      'Name'          => $params['Name'],
      'Email'         => $params['Email'],
      'AccessLevel'   => $params['AccessLevel'],
    )
  );

  $http_answer = d4os_http_request($simian_url, array('Content-Type' => 'application/json'), 'POST', $values);
  if ($http_answer->code == 200) {
    $result = json_decode($http_answer->data);
    if ($result->Success == 'true') {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  else {
    return FALSE;
  }
}

function _d4os_io_simian_add_identity($params) {
  global $simian_url;
  $values = drupal_to_js(
    array(
      'RequestMethod' => 'AddIdentity',
      'Identifier'    => $params['Identifier'],
      'Credential'    => $params['Credential'],
      'Type'          => $params['Type'],
      'UserID'        => $params['UserID'],
    )
  );

  $http_answer = d4os_http_request($simian_url, array('Content-Type' => 'application/json'), 'POST', $values);
  if ($http_answer->code == 200) {
    $result = json_decode($http_answer->data);
    if ($result->Success == 'true') {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  else {
    return FALSE;
  }
}

function _d4os_io_simian_add_inventory($params) {
  global $simian_url;
  $values = drupal_to_js(
    array(
      'RequestMethod' => 'AddInventory',
      'OwnerID'       => $params['OwnerID'],
    )
  );

  $http_answer = d4os_http_request($simian_url, array('Content-Type' => 'application/json'), 'POST', $values);
  if ($http_answer->code == 200) {
    $result = json_decode($http_answer->data);
    if ($result->Success == 'true') {
      return TRUE;
    }
    else {
      return $result->Message;
    }
  }
  else {
    return FALSE;
  }
}

function _d4os_io_simian_remove_user($params) {
  global $simian_url;
  $values = drupal_to_js(
    array(
      'RequestMethod' => 'RemoveUser',
      'UserID'        => $params['UserID'],
    )
  );

  $http_answer = d4os_http_request($simian_url, array('Content-Type' => 'application/json'), 'POST', $values);
  if ($http_answer->code == 200) {
    $result = json_decode($http_answer->data);
    if ($result->Success == 'true') {
      return TRUE;
    }
    else {
      return $result->Message;
    }
  }
  else {
    return FALSE;
  }
}

function _d4os_io_simian_remove_identity($params) {
  global $simian_url;
  $values = drupal_to_js(
    array(
      'RequestMethod' => 'RemoveIdentity',
      'Identifier'    => $params['Identifier'],
      'Type'          => $params['Type'],
    )
  );

  $http_answer = d4os_http_request($simian_url, array('Content-Type' => 'application/json'), 'POST', $values);
  if ($http_answer->code == 200) {
    $result = json_decode($http_answer->data);
    if ($result->Success == 'true') {
      return TRUE;
    }
    else {
      return $result->Message;
    }
  }
  else {
    return FALSE;
  }
}
