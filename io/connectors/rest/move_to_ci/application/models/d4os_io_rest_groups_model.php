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

class D4OS_IO_rest_Groups_Model extends CI_Model {

  public $response;
  public $groups_db;

  function __construct() {
    parent::__construct();
    $this->groups_db = $this->load->database('groups', TRUE);
    $this->response = new stdClass;
    $this->response->messages = array();
    $this->response->success = false;
  }

  function group_exists($uuid) {
    return $this->groups_db->get_where('osgroup', array('GroupID' => $uuid))->num_rows() > 0;
  }

  function delete_user($uuid) {
    $this->removeAgentFromGroups($uuid);
    return TRUE;
  }

  function add_user_to_group($user_uuid, $group_uuid) {
    $this->addAgentToGroup(array('AgentID' => $user_uuid, 'GroupID' => $group_uuid));
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
    $query = $this->groups_db->get_where('osgroupmembership', array('AgentID' => $agentID, 'GroupID' => $groupID));

    // If not a member, add membership, select role (defaults to uuidZero, or everyone role)
    if ($query->num_rows() < 1) {
      $values = array(
        'GroupID' => $groupID,
        'AgentID' => $agentID,
        'Contribution' => 0,
        'ListInProfile' => 1,
        'AcceptNotices' => 1,
        'SelectedRoleID' => $roleID
      );
      $this->groups_db->insert('osgroupmembership', $values);
    }

    // Make sure they're in the Everyone role
    $result = $this->addAgentToGroupRole(array("GroupID" => $groupID, "RoleID" => UUID_ZERO, "AgentID" => $agentID));

    // Make sure they're in specified role, if they were invited
    if ($roleID != UUID_ZERO) {
      $result = $this->addAgentToGroupRole(array("GroupID" => $groupID, "RoleID" => $roleID, "AgentID" => $agentID));
    }

    //Set the role they were invited to as their selected role
    $this->setAgentGroupSelectedRole(array('AgentID' => $agentID, 'RoleID' => $roleID, 'GroupID' => $groupID));

    $this->response->success = TRUE;
    $this->response->messages[] = 'User added to group ' . $groupID;
  }

  function addAgentToGroupRole($params) {
    $agentID = $params["AgentID"];
    $groupID = $params["GroupID"];
    $roleID = $params["RoleID"];

    // Check if agent already a member
    $query = $this->groups_db->get_where('osgrouprolemembership', array('AgentID' => $agentID, 'RoleID' => $roleID, 'GroupID' => $groupID));

    if ($query->num_rows() < 1) {
      $values = array(
        'GroupID' => $groupID,
        'RoleID' => $roleID,
        'AgentID' => $agentID
      );
      $this->groups_db->insert('osgrouprolemembership', $values);
    }

    $this->response->success = TRUE;
    $this->response->messages[] = 'User added to group role ' . $roleID;
  }

  function setAgentGroupSelectedRole($params) {
    $agentID = $params["AgentID"];
    $groupID = $params["GroupID"];
    $roleID = $params["RoleID"];

    $values = array(
      'SelectedRoleID' => $roleID
    );
    $where = array(
      'AgentID' => $agentID,
      'GroupID' => $groupID
    );
    $this->groups_db->update('osgroupmembership', $values, $where);

    $this->response->success = TRUE;
    $this->response->messages[] = 'Group role set to user';
  }

  function getAgentGroups($uuid) {
    $groups = array();
    $query = $this->groups_db->get_where('osgroupmembership', array('AgentID' => $uuid));
    if ($query->num_rows() > 0) {
      foreach ($query->result() as $row) {
        $groups[] = $row->GroupID;
      }
    }
    if (count($groups) == 0) {
      $this->response->messages[] = 'User has no group.';
      return;
    }
    else {
      $this->response->success = TRUE;
      $this->response->data = $groups;
      return $groups;
    }
  }

  function removeAgentFromGroups($uuid) {
    // get agent's groups
    $groups = $this->getAgentGroups($uuid);
    if (is_array($groups)) {
      if (count($groups)) {
        foreach ($groups as $group) {
          $this->removeAgentFromGroup(array('AgentID' => $uuid, 'GroupID' => $group));
        }
      }
    }
    $this->groups_db->delete('osgroupinvite', array('AgentID' => $uuid));
    $this->response->messages[] = 'User removed from groupinvites.';
  }

  function removeAgentFromGroup($params) {
    $agentID = $params["AgentID"];
    $groupID = $params["GroupID"];

    // 1. If group is agent's active group, change active group to uuidZero
    // 2. Remove Agent from group (osgroupmembership)
    // 3. Remove Agent from all of the groups roles (osgrouprolemembership)

    $this->groups_db->update('osagent', array('ActiveGroupID' => UUID_ZERO), array('AgentID' => $agentID, 'ActiveGroupID' => $groupID));
    $this->groups_db->delete('osgroupmembership', array('AgentID' => $agentID, 'GroupID' => $groupID));
    $this->groups_db->delete('osgrouprolemembership', array('AgentID' => $agentID, 'GroupID' => $groupID));

    $this->response->success = TRUE;
    $this->response->messages[] = 'User removed from group ' . $groupID;
  }

}