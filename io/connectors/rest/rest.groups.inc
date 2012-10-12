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
class D4OS_IO_rest_Groups implements D4OS_IO_Groups_Interface {

  public $response;
  public $values;

  function __construct() {
    $this->values = array(
    );
  }

  function ping() {
    $this->response->success = false;
    $result = d4os_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_groups/ping/format/json');
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      if ($data->status === 'success') {
        $this->response->success = TRUE;
      }
    }
    return;
  }

  function group_exists($uuid) {
    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_groups/group_exists/uuid/' . $uuid . '/format/json';
    $result = d4os_http_request($url);
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      return TRUE;
    }
    return FALSE;
  }

  function add_user_to_group($user_uuid, $group_uuid) {
    if ($user_uuid == UUID_ZERO || empty($user_uuid) || $group_uuid == UUID_ZERO || empty($group_uuid)) {
      drupal_set_message(t('Could not add user to group.', 'error'));
      return;
    }
    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_groups/add_user_to_group/user_uuid/' . $user_uuid . '/group_uuid/' . $group_uuid . '/format/json';
    $result = d4os_http_request($url);
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      drupal_set_message(t('User added to group.'));
      return TRUE;
    }
    return FALSE;
  }

  function delete_user($uuid) {
    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_groups/delete_user/uuid/' . $uuid . '/format/json';
    $result = d4os_http_request($url);
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      drupal_set_message(t('User removed from groups.'));
      return TRUE;
    }
    return FALSE;
  }

}
