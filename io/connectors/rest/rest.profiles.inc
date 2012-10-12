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
class D4OS_IO_rest_Profiles implements D4OS_IO_Profiles_Interface {

  public $response;
  public $values;

  function __construct() {
    $this->values = array(
    );
  }

  function ping() {
    $this->response->success = false;
    $result = d4os_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/ping/format/json');
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      if ($data->status === 'success') {
        $this->response->success = true;
      }
    }
    return;
  }

  function load_profile($values) {
    $this->response->success = false;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/load_profile/' . $params . 'format/json';
    $result = d4os_http_request($url);

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = true;
      if (is_array($data)) {
        return $data[0];
      }
    }
    return null;
  }

  function save_profile($values) {
    $data = array("data" => json_encode($values));
    $response = d4os_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/save_profile/format/json'
        , array('Content-Type' => 'application/x-www-form-urlencoded')
        , 'POST', http_build_query($data, '', '&'));
    if (isset($response) && in_array($response->code, array('200'))) {
      drupal_set_message(t('User profile added'));
      return array(
        'success' => TRUE,
        'errorMessage' => "",
      );
    }
    return array(
      'success' => FALSE,
      'errorMessage' => "",
    );
  }

  function delete_profile($useruuid) {
    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/delete_profile/useruuid/' . $useruuid . '/format/json';
    $result = d4os_http_request($url);

    if (isset($result) && in_array($result->code, array('200'))) {
      drupal_set_message(t('User removed from profiles.'));
      return TRUE;
    }
    return FALSE;
  }

  /*
   * Profile services
   */

  function avatarclassifiedsrequest($values) {
    $this->response->success = false;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatarclassifiedsrequest/' . $params . 'format/json';
    $result = d4os_http_request($url);

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = true;
      if (is_array($data)) {
        return $data;
      }
    }
    return null;
  }

  function classified_update($values) {
    $data = array("data" => json_encode($values));
    $response = d4os_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/classified_update/format/json'
        , array('Content-Type' => 'application/x-www-form-urlencoded')
        , 'POST', http_build_query($data, '', '&'));
    if (isset($response) && in_array($response->code, array('200'))) {
      return array(
        'success' => TRUE,
        'errorMessage' => "",
      );
    }
    return array(
      'success' => FALSE,
      'errorMessage' => "",
    );
  }

  function classified_delete($values) {
    $id = $values['classifiedID'];
    $this->response->success = false;
    $result = d4os_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/classified_delete/classifiedID/' . rawurlencode($id) . '/format/json');
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      if ($data->status === 'success') {
        $this->response->success = true;
        if (is_array($data->messages)) {
          foreach ($data->messages as $message) {
            drupal_set_message($message);
          }
        }
      }
    }
    return;
  }

  function avatarpicksrequest($values) {
    $this->response->success = false;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatarpicksrequest/' . $params . 'format/json';
    $result = d4os_http_request($url);

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = true;
      if (is_array($data)) {
        return $data;
      }
    }
    return null;
  }

  function pickinforequest($values) {
    $this->response->success = false;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/pickinforequest/' . $params . 'format/json';
    $result = d4os_http_request($url);

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = true;
      if (is_array($data)) {
        return $data;
      }
    }
    return null;
  }

  function picks_update($values) {
    $data = array("data" => json_encode($values));
    $response = d4os_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/picks_update/format/json'
        , array('Content-Type' => 'application/x-www-form-urlencoded')
        , 'POST', http_build_query($data, '', '&'));
    if (isset($response) && in_array($response->code, array('200'))) {
      return array(
        'success' => TRUE,
        'errorMessage' => "",
      );
    }
    return array(
      'success' => FALSE,
      'errorMessage' => "",
    );
  }

  function picks_delete($values) {
    $id = $values['pick_id'];
    $this->response->success = false;
    $result = d4os_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/picks_delete/pick_id/' . rawurlencode($id) . '/format/json');
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      if ($data->status === 'success') {
        $this->response->success = true;
        if (is_array($data->messages)) {
          foreach ($data->messages as $message) {
            drupal_set_message($message);
          }
        }
      }
    }
    return;
  }

  function avatarnotesrequest($values) {
    $this->response->success = false;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatarnotesrequest/' . $params . 'format/json';
    $result = d4os_http_request($url);

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = true;
      if (is_array($data)) {
        return $data;
      }
    }
    return null;
  }

  function avatar_notes_update($values) {
    $data = array("data" => json_encode($values));
    $response = d4os_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatar_notes_update/format/json'
        , array('Content-Type' => 'application/x-www-form-urlencoded')
        , 'POST', http_build_query($data, '', '&'));
    if (isset($response) && in_array($response->code, array('200'))) {
      return array(
        'success' => TRUE,
        'errorMessage' => "",
      );
    }
    return array(
      'success' => FALSE,
      'errorMessage' => "",
    );
  }

  function avatar_properties_request($values) {
    $this->response->success = false;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatar_properties_request/' . $params . 'format/json';
    $result = d4os_http_request($url);

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = true;
      if (is_array($data)) {
        return $data;
      }
    }
    return null;
  }

  function avatar_properties_update($values) {
    $data = array("data" => json_encode($values));
    $response = d4os_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatar_properties_update/format/json'
        , array('Content-Type' => 'application/x-www-form-urlencoded')
        , 'POST', http_build_query($data, '', '&'));

    if (isset($response) && in_array($response->code, array('200'))) {
      return array(
        'success' => TRUE,
        'errorMessage' => "",
      );
    }
    return array(
      'success' => FALSE,
      'errorMessage' => "",
    );
  }

  function avatar_interests_request($values) {
    $this->response->success = false;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatar_interests_request/' . $params . 'format/json';
    $result = d4os_http_request($url);

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = true;
      if (is_array($data)) {
        return $data;
      }
    }
    return null;
  }

  function avatar_interests_update($values) {
    $data = array("data" => json_encode($values));
    $response = d4os_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatar_interests_update/format/json'
        , array('Content-Type' => 'application/x-www-form-urlencoded')
        , 'POST', http_build_query($data, '', '&'));
    if (isset($response) && in_array($response->code, array('200'))) {
      return array(
        'success' => TRUE,
        'errorMessage' => "",
      );
    }
    return array(
      'success' => FALSE,
      'errorMessage' => "",
    );
  }

  function user_preferences_request($values) {
    $this->response->success = false;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $url = D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/user_preferences_request/' . $params . 'format/json';
    $result = d4os_http_request($url);

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = true;
      if (is_array($data)) {
        return $data;
      }
    }
    return null;
  }

  function user_preferences_update($values) {
    $data = array("data" => json_encode($values));
    $response = d4os_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/user_preferences_update/format/json'
        , array('Content-Type' => 'application/x-www-form-urlencoded')
        , 'POST', http_build_query($data, '', '&'));
    if (isset($response) && in_array($response->code, array('200'))) {
      return array(
        'success' => TRUE,
        'errorMessage' => "",
      );
    }
    return array(
      'success' => FALSE,
      'errorMessage' => "",
    );
  }

}
