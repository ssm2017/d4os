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
class D4OS_IO_db_070_Groups implements D4OS_IO_groups_Interface {

  public $response;
  public $values;

  function __construct() {
    $this->values = array();
  }

  function ping() {
    $this->response->success = d4os_io_db_070_mysql_is_alive('os_groups', 'Robust');
    return;
  }

  function group_exists($uuid) {
    d4os_io_db_070_set_active('os_groups');
    $group_exists = db_query("SELECT count(GroupID) FROM {osgroup} WHERE GroupID = :GroupID", array(':GroupID' => $uuid))->fetchField();
    d4os_io_db_070_set_active('default');
    if ($group_exists) {
      return TRUE;
    }
    return FALSE;
  }

  function add_user_to_group($user_uuid, $group_uuid) {
    $this->addAgentToGroup(array('AgentID' => $user_uuid, 'GroupID' => $group_uuid));
    return TRUE;
  }

  function delete_user($uuid) {
    $this->removeAgentFromGroups($uuid);
    return TRUE;
  }

  function addAgentToGroup($params) {
    $agentID = $params["AgentID"];
    $groupID = $params["GroupID"];

    $roleID = UUID_ZERO;
    if (isset($params["RoleID"])) {
      $roleID = $params["RoleID"];
    }

    // Check if agent already a member
    $query = "SELECT count(AgentID) as isMember FROM osgroupmembership WHERE AgentID = :AgentID AND GroupID = :GroupID";
    d4os_io_db_070_set_active('os_groups');
    $result = db_query($query, array(':AgentID' => $agentID, ':GroupID' => $groupID))->fetchField();
    d4os_io_db_070_set_active('default');

    // If not a member, add membership, select role (defaults to uuidZero, or everyone role)
    if ($result && $result === 0) {
      $values = array(
        'GroupID' => $groupID,
        'AgentID' => $agentID,
        'Contribution' => 0,
        'ListInProfile' => 1,
        'AcceptNotices' => 1,
        'SelectedRoleID' => $roleID
      );

      d4os_io_db_070_set_active('os_groups');
      db_insert('osgroupmembership')
          ->fields($values)
          ->execute();
      d4os_io_db_070_set_active('default');
    }

    // Make sure they're in the Everyone role
    $result = $this->addAgentToGroupRole(array("GroupID" => $groupID, "RoleID" => UUID_ZERO, "AgentID" => $agentID));

    // Make sure they're in specified role, if they were invited
    if ($roleID != UUID_ZERO) {
      $result = $this->addAgentToGroupRole(array("GroupID" => $groupID, "RoleID" => $roleID, "AgentID" => $agentID));
    }

    //Set the role they were invited to as their selected role
    $this->setAgentGroupSelectedRole(array('AgentID' => $agentID, 'RoleID' => $roleID, 'GroupID' => $groupID));

    drupal_set_message(t('User added to group.'));

    return array("success" => "true");
  }

  function addAgentToGroupRole($params) {
    $agentID = $params["AgentID"];
    $groupID = $params["GroupID"];
    $roleID = $params["RoleID"];

    // Check if agent already a member
    $query = "SELECT count(AgentID) as isMember FROM osgrouprolemembership WHERE AgentID = :AgentID AND RoleID = :RoleID AND GroupID = :GroupID";
    d4os_io_db_070_set_active('os_groups');
    $result = db_query($query, array(':AgentID' => $agentID, ':RoleID' => $roleID, ':GroupID' => $groupID))->fetchField();
    d4os_io_db_070_set_active('default');

    if ($result && $result === 0) {
      $values = array(
        'GroupID' => $groupID,
        'RoleID' => $roleID,
        'AgentID' => $agentID
      );

      d4os_io_db_070_set_active('os_groups');
      db_insert('osgrouprolemembership')
          ->fields($values)
          ->execute();
      d4os_io_db_070_set_active('default');
    }

    return array("success" => "true");
  }

  function setAgentGroupSelectedRole($params) {
    $agentID = $params["AgentID"];
    $groupID = $params["GroupID"];
    $roleID = $params["RoleID"];

    d4os_io_db_070_set_active('os_groups');
    db_update('osgroupmembership')
        ->fields(array(
          'SelectedRoleID' => $roleID
        ))
        ->condition('AgentID', $agentID)
        ->condition('GroupID', $groupID)
        ->execute();
    d4os_io_db_070_set_active('default');

    return array('success' => 'true');
  }

  function getAgentGroups($uuid) {
    $groups = array();
    d4os_io_db_070_set_active('os_groups');
    $results = db_query("SELECT GroupID FROM {osgroupmembership} WHERE AgentID = :AgentID", array(':AgentID' => $uuid));
    foreach ($results as $group) {
      $groups[] = $group->GroupID;
    }
    d4os_io_db_070_set_active('default');
    if (count($groups)) {
      return $groups;
    }
    else {
      return NULL;
    }
  }

  function removeAgentFromGroups($uuid) {
    // get agent's groups
    $groups = $this->getAgentGroups($uuid);

    // remove agent from groups
    if (is_array($groups) && count($groups)) {
      foreach ($groups as $group) {
        $this->removeAgentFromGroup(array('GroupID' => $group, 'AgentID' => $uuid));
      }
    }

    // remove agent from group invites
    d4os_io_db_070_set_active('os_groups');
    db_query("DELETE FROM {osgroupinvite} WHERE AgentID = :AgentID", array('AgentID' => $uuid));
    d4os_io_db_070_set_active('default');

    drupal_set_message(t('User removed from group invites.'));
  }

  function removeAgentFromGroup($params) {
    $agentID = $params["AgentID"];
    $groupID = $params["GroupID"];

    // 1. If group is agent's active group, change active group to uuidZero
    // 2. Remove Agent from group (osgroupmembership)
    // 3. Remove Agent from all of the groups roles (osgrouprolemembership)

    $sql = " UPDATE osagent "
        . " SET ActiveGroupID = :ActiveGroupID"
        . " WHERE AgentID = :AgentID AND ActiveGroupID = :ActiveGroupID";
    d4os_io_db_070_set_active('os_groups');
    db_query($sql, array(':ActiveGroupID' => UUID_ZERO, ':AgentID' => $agentID, ':ActiveGroupID' => $groupID));
    d4os_io_db_070_set_active('default');

    $sql = " DELETE FROM osgroupmembership "
        . " WHERE AgentID = :AgentID AND GroupID = :GroupID";
    d4os_io_db_070_set_active('os_groups');
    db_query($sql, array(':AgentID' => $agentID, ':GroupID' => $groupID));
    d4os_io_db_070_set_active('default');

    $sql = " DELETE FROM osgrouprolemembership "
        . " WHERE AgentID = :AgentID AND GroupID = :GroupID";
    d4os_io_db_070_set_active('os_groups');
    db_query($sql, array(':AgentID' => $agentID, ':GroupID' => $groupID));
    d4os_io_db_070_set_active('default');

    drupal_set_message(t('User removed from group.'));
  }

}
