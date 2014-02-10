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

/*
FLAGS :
1 = see online
2 = see on map
4 = allow modify objects
*/
/*
HYPERGRID FRIEND :
UUID;HGURI/;FirstName LastName;something
*/
class D4OS_IO_db_070_Friends implements D4OS_IO_Friends_Interface {

  public $response;
  public $values;

  function __construct() {
    $this->response = new stdClass();
    $this->values = array();
  }

  function ping() {
    $this->response->success = d4os_io_db_070_mysql_is_alive('os_robust', 'Robust');
    return;
  }

  function get_friends($uuid) {
    $items = array();
    $query = "SELECT concat(ua.FirstName, ' ', ua.LastName) AS name, f.*"
          . " FROM {Friends} AS f"
          . " LEFT JOIN {UserAccounts} AS ua ON ua.PrincipalID=f.Friend"
          . " WHERE f.PrincipalID = :uuid"
          . " ORDER BY concat(ua.FirstName, ' ', ua.LastName)";
    d4os_io_db_070_set_active('os_robust');
    $results = db_query($query, array(':uuid' => $uuid));
    foreach ($results as $result) {
      $items[$result->Friend] = $result;
    }
    d4os_io_db_070_set_active('default');
    return $items;
  }

  function get_friend($uuid, $friend) {
    $query = "SELECT concat(ua.FirstName, ' ', ua.LastName) AS name, f.*"
          . " FROM {Friends} AS f"
          . " LEFT JOIN {UserAccounts} AS ua ON ua.PrincipalID=f.Friend"
          . " WHERE f.PrincipalID = :uuid AND f.Friend = :friend";
    d4os_io_db_070_set_active('os_robust');
    $friend = db_query($query, array(':uuid' => $uuid, ':friend' => $friend))->fetch();
    d4os_io_db_070_set_active('default');
    if ($friend) {
      return $friend;
    }
    return NULL;
  }

  function delete_friend($uuid, $friend) {
    // delete one side
    d4os_io_db_070_set_active('os_robust');
    $num_deleted = db_delete('Friends')
      ->condition('PrincipalID', $uuid, '=')
      ->condition('Friend', $friend, '=')
      ->execute();
    d4os_io_db_070_set_active('default');
    // delete the other side
    d4os_io_db_070_set_active('os_robust');
    $num_deleted = db_delete('Friends')
      ->condition('PrincipalID', $friend, '=')
      ->condition('Friend', $uuid, '=')
      ->execute();
    d4os_io_db_070_set_active('default');
    return $num_deleted;
  }

  function set_flags($uuid, $friend, $flags) {
    d4os_io_db_070_set_active('os_robust');
    $num_updated = db_update('Friends')
      ->fields(array(
        'Flags' => $flags
      ))
      ->condition('PrincipalID', $uuid, '=')
      ->condition('Friend', $friend, '=')
      ->execute();
    d4os_io_db_070_set_active('default');
    return $num_updated;
  }

  function get_online_friends($uuid) {
    $items = array();
    $query = "SELECT concat(ua.FirstName, ' ',ua.LastName) AS name, ua.PrincipalID"
          . " FROM {Friends} AS f"
          . " LEFT JOIN {UserAccounts} AS ua ON ua.PrincipalID=f.Friend"
          . " LEFT JOIN {Presence} AS p ON p.UserID=f.Friend"
          . " LEFT JOIN {Friends} AS f2 ON f2.PrincipalID=f.Friend"
          . " WHERE f.PrincipalID=:uuid AND p.UserID is not null AND f2.Friend=:uuid AND f2.Flags IN (1, 3, 5, 7)"
          . " ORDER BY concat(ua.FirstName, ' ', ua.LastName)";
    d4os_io_db_070_set_active('os_robust');
    $results = db_query($query, array(':uuid' => $uuid));
    foreach ($results as $result) {
      $items[$result->PrincipalID] = $result;
    }
    d4os_io_db_070_set_active('default');
    return $items;
  }
}
