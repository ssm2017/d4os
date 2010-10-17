<?php
/**
 * @package    d4os_io_db_070
 * @subpackage   friends
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

function d4os_io_db_070_os_friends_get_friends($uuid) {
  $data = array();

  $query = "SELECT *, CONCAT_WS(' ', ua.FirstName, ua.LastName) AS name FROM {UserAccounts} AS ua"
        . " LEFT JOIN {Friends} AS f ON f.PrincipalID='%s'"
        . " WHERE ua.PrincipalID=f.Friend";
  d4os_io_db_070_set_active('os_robust');
  $result = db_query($query, $uuid);
  while ($friend = db_fetch_object($result)) {
    $data[] = $friend;
  }
  d4os_io_db_070_set_active('default');

  if (count($data) == 0) {
    return array(
      'success' => FALSE,
      'message' => t('No friends found for this user.')
    );
  }

  return array(
    'success' => TRUE,
    'data'    => $data
  );
}

function d4os_io_db_070_os_friends_get_online_friends($uuid) {
  $data = array();

  $query = "SELECT *, CONCAT_WS(' ', ua.FirstName, ua.LastName) AS name FROM {UserAccounts} AS ua"
        . " LEFT JOIN {Friends} AS f ON f.PrincipalID='%s'"
        . " LEFT JOIN {Presence} AS p ON p.UserID=f.Friend"
        . " WHERE ua.PrincipalID=p.UserID";
  d4os_io_db_070_set_active('os_robust');
  $result = db_query($query, $uuid);
  while ($friend = db_fetch_object($result)) {
    $data[] = $friend;
  }
  d4os_io_db_070_set_active('default');

  if (count($data) == 0) {
    return array(
      'success' => FALSE,
      'message' => t('No friends online.')
    );
  }

  return array(
    'success' => TRUE,
    'data'    => $data
  );
}
