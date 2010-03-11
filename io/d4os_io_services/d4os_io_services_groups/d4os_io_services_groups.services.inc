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

/*
 * The code in this script was inspired and mostly copied from flotsam xmlrpc groups implementation
 * http://code.google.com/p/flotsam/wiki/XmlRpcGroups
 */

global $d4os_io_services_groups_log_level;
$d4os_io_services_groups_log_level = variable_get('d4os_io_services_groups_log_level', 0);

global $groupPowers, $requestingAgent, $common_sig;
$groupPowers = array(
  'None' => '0',
  /// <summary>Can send invitations to groups default role</summary>
  'Invite' => '2',
  /// <summary>Can eject members from group</summary>
  'Eject' => '4',
  /// <summary>Can toggle 'Open Enrollment' and change 'Signup fee'</summary>
  'ChangeOptions' => '8',
  /// <summary>Can create new roles</summary>
  'CreateRole' => '16',
  /// <summary>Can delete existing roles</summary>
  'DeleteRole' => '32',
  /// <summary>Can change Role names, titles and descriptions</summary>
  'RoleProperties' => '64',
  /// <summary>Can assign other members to assigners role</summary>
  'AssignMemberLimited' => '128',
  /// <summary>Can assign other members to any role</summary>
  'AssignMember' => '256',
  /// <summary>Can remove members from roles</summary>
  'RemoveMember' => '512',
  /// <summary>Can assign and remove abilities in roles</summary>
  'ChangeActions' => '1024',
  /// <summary>Can change group Charter, Insignia, 'Publish on the web' and which
  /// members are publicly visible in group member listings</summary>
  'ChangeIdentity' => '2048',
  /// <summary>Can buy land or deed land to group</summary>
  'LandDeed' => '4096',
  /// <summary>Can abandon group owned land to Governor Linden on mainland, or Estate owner for
  /// private estates</summary>
  'LandRelease' => '8192',
  /// <summary>Can set land for-sale information on group owned parcels</summary>
  'LandSetSale' => '16384',
  /// <summary>Can subdivide and join parcels</summary>
  'LandDivideJoin' => '32768',
  /// <summary>Can join group chat sessions</summary>
  'JoinChat' => '65536',
  /// <summary>Can toggle "Show in Find Places" and set search category</summary>
  'FindPlaces' => '131072',
  /// <summary>Can change parcel name, description, and 'Publish on web' settings</summary>
  'LandChangeIdentity' => '262144',
  /// <summary>Can set the landing point and teleport routing on group land</summary>
  'SetLandingPoint' => '524288',
  /// <summary>Can change music and media settings</summary>
  'ChangeMedia' => '1048576',
  /// <summary>Can toggle 'Edit Terrain' option in Land settings</summary>
  'LandEdit' => '2097152',
  /// <summary>Can toggle various About Land > Options settings</summary>
  'LandOptions' => '4194304',
  /// <summary>Can always terraform land, even if parcel settings have it turned off</summary>
  'AllowEditLand' => '8388608',
  /// <summary>Can always fly while over group owned land</summary>
  'AllowFly' => '16777216',
  /// <summary>Can always rez objects on group owned land</summary>
  'AllowRez' => '33554432',
  /// <summary>Can always create landmarks for group owned parcels</summary>
  'AllowLandmark' => '67108864',
  /// <summary>Can use voice chat in Group Chat sessions</summary>
  'AllowVoiceChat' => '134217728',
  /// <summary>Can set home location on any group owned parcel</summary>
  'AllowSetHome' => '268435456',
  /// <summary>Can modify public access settings for group owned parcels</summary>
  'LandManageAllowed' => '536870912',
  /// <summary>Can manager parcel ban lists on group owned land</summary>
  'LandManageBanned' => '1073741824',
  /// <summary>Can manage pass list sales information</summary>
  'LandManagePasses' => '2147483648',
  /// <summary>Can eject and freeze other avatars on group owned land</summary>
  'LandEjectAndFreeze' => '4294967296',
  /// <summary>Can return objects set to group</summary>
  'ReturnGroupSet' => '8589934592',
  /// <summary>Can return non-group owned/set objects</summary>
  'ReturnNonGroup' => '17179869184',
  /// <summary>Can landscape using Linden plants</summary>
  'LandGardening' => '34359738368',
  /// <summary>Can deed objects to group</summary>
  'DeedObject' => '68719476736',
  /// <summary>Can moderate group chat sessions</summary>
  'ModerateChat' => '137438953472',
  /// <summary>Can move group owned objects</summary>
  'ObjectManipulate' => '274877906944',
  /// <summary>Can set group owned objects for-sale</summary>
  'ObjectSetForSale' => '549755813888',
  /// <summary>Pay group liabilities and receive group dividends</summary>
  'Accountable' => '1099511627776',
  /// <summary>Can send group notices</summary>
  'SendNotices'    => '4398046511104',
  /// <summary>Can receive group notices</summary>
  'ReceiveNotices' => '8796093022208',
  /// <summary>Can create group proposals</summary>
  'StartProposal' => '17592186044416',
  /// <summary>Can vote on group proposals</summary>
  'VoteOnProposal' => '35184372088832',
  /// <summary>Can return group owned objects</summary>
  'ReturnGroupOwned' => '281474976710656',
  /// <summary>Members are visible to non-owners</summary>
  'RoleMembersVisible' => '140737488355328'
);

// This is filled in by secure()
$requestingAgent = UUID_ZERO;

// Use a common signature for all the group functions  ->  struct foo($struct)
$common_sig = array(array($xmlrpcStruct, $xmlrpcStruct));

function d4os_io_services_groups_createGroup($params, $inworld = TRUE) {
  global $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'createGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }
  $groupID        = $params["GroupID"];
  $name           = addslashes( $params["Name"] );
  $charter        = addslashes( $params["Charter"] );
  $insigniaID     = $params["InsigniaID"];
  $founderID      = $params["FounderID"];
  $membershipFee  = $params["MembershipFee"];
  $openEnrollment = $params["OpenEnrollment"];
  $showInList     = $params["ShowInList"];
  $allowPublish   = $params["AllowPublish"];
  $maturePublish  = $params["MaturePublish"];
  $ownerRoleID    = $params["OwnerRoleID"];
  $everyonePowers = $params["EveryonePowers"];
  $ownersPowers   = $params["OwnersPowers"];

  // do the job
  $group_created = module_invoke_all('os_groups_services_createGroup', $params);
  if (isset($group_created['error'])) {
    return $group_created;
  }

  // Create Everyone Role
  // NOTE: FIXME: This is a temp fix until the libomv enum for group powers is fixed in OpenSim
  $result = _d4os_io_services_groups_addRoleToGroup(array('GroupID' => $groupID, 'RoleID' => UUID_ZERO, 'Name' => 'Everyone', 'Description' => 'Everyone in the group is in the everyone role.', 'Title' => "Member of $name", 'Powers' => $everyonePowers));
  if (isset($result['error'])) {
    return $result;
  }

  // Create Owner Role
  $result = _d4os_io_services_groups_addRoleToGroup(array('GroupID' => $groupID, 'RoleID' => $ownerRoleID, 'Name' => 'Owners', 'Description' => "Owners of $name", 'Title' => "Owner of $name", 'Powers' => $ownersPowers));
  if (isset($result['error'])) {
    return $result;
  }

  // Add founder to group, will automatically place them in the Everyone Role, also places them in specified Owner Role
  $result = _d4os_io_services_groups_addAgentToGroup(array('AgentID' => $founderID, 'GroupID' => $groupID, 'RoleID' => $ownerRoleID));
  if (isset($result['error'])) {
    return $result;
  }

  // Select the owner's role for the founder
  $result = _d4os_io_services_groups_setAgentGroupSelectedRole(array('AgentID' => $founderID, 'RoleID' => $ownerRoleID, 'GroupID' => $groupID));
  if (isset($result['error'])) {
    return $result;
  }

  // Set the new group as the founder's active group
  $result = _d4os_io_services_groups_setAgentActiveGroup(array('AgentID' => $founderID, 'GroupID' => $groupID));
  if (isset($result['error'])) {
    return $result;
  }

  $return = d4os_io_services_groups_getGroup(array("GroupID"=>$groupID));
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'createGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  if ($inworld) {
    module_invoke_all('os_group_grid_create', $return);
  }
  return $return;
}

// Private method, does not include security, to only be called from places that have already verified security
function _d4os_io_services_groups_addRoleToGroup($params, $inworld = TRUE) {
  // do the job
  return module_invoke_all('os_groups_services_addRoleToGroup', $params);
}

function d4os_io_services_groups_addRoleToGroup($params, $inworld = TRUE) {
  global $groupPowers, $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'addRoleToGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }


  // Verify the requesting agent has permission
  if (is_array($error = d4os_io_services_groups_checkGroupPermission($params['GroupID'], $groupPowers['CreateRole']))) {
    return $error;
  }

  $return = _d4os_io_services_groups_addRoleToGroup($params, $inworld);

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'addRoleToGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_updateGroupRole($params, $inworld = TRUE) {
  global $groupPowers, $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'updateGroupRole',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  // Verify the requesting agent has permission
  if (is_array($error = d4os_io_services_groups_checkGroupPermission($params['GroupID'], $groupPowers['RoleProperties']))) {
    return $error;
  }

  // do the job
  $output = module_invoke_all('os_groups_services_updateGroupRole', $params);
  if (isset($output['error'])) {
    return $output;
  }

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'updateGroupRole',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $output;
}

function d4os_io_services_groups_removeRoleFromGroup($params, $inworld = TRUE) {
  global $groupPowers, $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'removeRoleFromGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  if (is_array($error = d4os_io_services_groups_checkGroupPermission($params['GroupID'], $groupPowers['RoleProperties']))) {
    return $error;
  }

  // do the job
  $return = module_invoke_all('os_groups_services_removeRoleFromGroup', $params);

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'removeRoleFromGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getGroup($params, $inworld = TRUE) {
  global $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

  $return = _d4os_io_services_groups_getGroup($params, $inworld);

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function _d4os_io_services_groups_getGroup($params, $inworld = TRUE) {
  // do the job
  $group = module_invoke_all('os_groups_services_getGroup', $params);
  return $group;
}        

function d4os_io_services_groups_updateGroup($params, $inworld = TRUE) {
  global $groupPowers, $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'updateGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  if (is_array($error = d4os_io_services_groups_checkGroupPermission($params["GroupID"], $groupPowers['ChangeOptions']))) {
    return $error;
  }

  // do the job
  $return = module_invoke_all('os_groups_services_updateGroup', $params);

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'updateGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_findGroups($params, $inworld = TRUE) {
  global $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'findGroups',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if(is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

  // do the job
  $return = module_invoke_all('os_groups_services_findGroups', $params);

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'findGroups',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function _d4os_io_services_groups_setAgentActiveGroup($params, $inworld = TRUE) {
  // do the job
  return module_invoke_all('os_groups_services_setAgentActiveGroup', $params);
}

function d4os_io_services_groups_setAgentActiveGroup($params, $inworld = TRUE) {
  global $requestingAgent, $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
       'type' => 'string',
      'data' => 'setAgentActiveGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  $agentID = $params['AgentID'];
  $groupID = $params['GroupID'];

  if (isset($requestingAgent) && ($requestingAgent != UUID_ZERO) && ($requestingAgent != $agentID)) {
    return array(
      'error' => "Agent can only change their own Selected Group Role",
      'params' => var_export($params, TRUE)
    );
  }

  $return = _d4os_io_services_groups_setAgentActiveGroup($params, $inworld);

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'setAgentActiveGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_addAgentToGroup($params, $inworld = TRUE) {
  global $groupPowers, $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'addAgentToGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  $groupID = $params["GroupID"];
  $agentID = $params["AgentID"];

  if (is_array($error = d4os_io_services_groups_checkGroupPermission($groupID, $groupPowers['AssignMember']))) {
    // If they don't have direct permission, check to see if the group is marked for open enrollment
    $groupInfo = _d4os_io_services_groups_getGroup( array ('GroupID'=>$groupID) );
  
    if( isset($groupInfo['error'])) {
      return $groupInfo;
    }

    if($groupInfo['OpenEnrollment'] != 1) {

      // do the job
      $inviteInfo = module_invoke_all('os_groups_services_addAgentToGroup', $params);
      if (isset($inviteInfo['error'])) {
        return $inviteInfo;
      }
  
      if(is_array($inviteInfo) && isset($inviteInfo['RoleID'])) {
        // if there is an invite, make sure we're adding the user to the role specified in the invite
        $params['RoleID'] = $inviteInfo['RoleID'];
      }
      else {
        // Not openenrollment, not invited, return permission denied error
        return $error;
      }
    }
  }

  $return = _d4os_io_services_groups_addAgentToGroup($params, $inworld);

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'addAgentToGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

// Private method, does not include security, to only be called from places that have already verified security
function _d4os_io_services_groups_addAgentToGroup($params, $inworld = TRUE) {
  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];
  
  $roleID  = UUID_ZERO;
  if (isset($params["RoleID"])) {
    $roleID = $params["RoleID"];
  }

  // do the job
  $agent_added = module_invoke_all('os_groups_services_addAgentToGroup_2', $params);
  if (isset($agent_added['error'])) {
    return $agent_added;
  }

  // Make sure they're in the Everyone role
  $result = _d4os_io_services_groups_addAgentToGroupRole(array("GroupID" => $groupID, "RoleID" => UUID_ZERO, "AgentID" => $agentID));
  if(isset($result['error'])) {
    return $result;
  }

  // Make sure they're in specified role, if they were invited
  if ($roleID != UUID_ZERO) {
    $result = _d4os_io_services_groups_addAgentToGroupRole(array("GroupID" => $groupID, "RoleID" => $roleID, "AgentID" => $agentID));
    if(isset($result['error'])) {
      return $result;
    }
  }

  //Set the role they were invited to as their selected role
  _d4os_io_services_groups_setAgentGroupSelectedRole(array('AgentID' => $agentID, 'RoleID' => $roleID, 'GroupID' => $groupID));
  
  // Set the group as their active group.
  // _d4os_io_services_groups_setAgentActiveGroup(array("GroupID" => $groupID, "AgentID" => $agentID));
  return array("success" => "true");
}

function d4os_io_services_groups_removeAgentFromGroup($params, $inworld = TRUE) {
  global $requestingAgent, $groupPowers, $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'removeAgentFromGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];

  // An agent is always allowed to remove themselves from a group -- so only check if the requesting agent is different then the agent being removed.
  if ($agentID != $requestingAgent) {
    if (is_array($error = d4os_io_services_groups_checkGroupPermission($groupID, $groupPowers['RemoveMember']))) {
      return $error;
    }
  }

  // do the job
  $return = module_invoke_all('os_groups_services_removeAgentFromGroup', $params);

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'removeAgentFromGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function _d4os_io_services_groups_addAgentToGroupRole($params, $inworld = TRUE) {
  // do the job
  return module_invoke_all('os_groups_services_addAgentToGroupRole', $params);
}

function d4os_io_services_groups_addAgentToGroupRole($params, $inworld = TRUE) {
  global $requestingAgent, $groupPowers, $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'addAgentToGroupRole',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];
  $roleID = $params["RoleID"];
  
  // do the job
  $ownerRoleInfo = module_invoke_all('os_groups_services_addAgentToGroupRole_2', $params);
  if (isset($ownerRoleInfo['error'])) {
    return $ownerRoleInfo;
  }

  if(!is_array($ownerRoleInfo) || !isset($ownerRoleInfo['OwnerRoleID'])) {
    return array(
      'error' => t("Group (!group) not found or Agent (!agent) is not in the owner's role", array('!group' => $groupID, '!agent' => $agentID)),
      'params' => var_export($params, TRUE)
    );
  }

  if (($ownerRoleInfo['OwnerRoleID'] == $roleID) && ($ownerRoleInfo['AgentID'] != $requestingAgent)) {
    return array(
      'error' => t("Requesting agent !requestingAgent is not a member of the Owners Role and cannot add members to the owners role.", array('!requestingAgent' => $requestingAgent)),
      'params' => var_export($params, TRUE)
    );
  }

  if (is_array($error = d4os_io_services_groups_checkGroupPermission($groupID, $groupPowers['AssignMember']))) {
    return $error;
  }
  
  $return = _d4os_io_services_groups_addAgentToGroupRole($params, $inworld);

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'addAgentToGroupRole',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_removeAgentFromGroupRole($params, $inworld = TRUE) {
  global $groupPowers, $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'removeAgentFromGroupRole',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  if (is_array($error = d4os_io_services_groups_checkGroupPermission($params["GroupID"], $groupPowers['AssignMember']))) {
    return $error;
  }
  
  // do the job
  $return = module_invoke_all('os_groups_services_removeAgentFromGroupRole', $params);

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'removeAgentFromGroupRole',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function _d4os_io_services_groups_setAgentGroupSelectedRole($params, $inworld = TRUE) {
  // do the job
  return module_invoke_all('os_groups_services_setAgentGroupSelectedRole', $params);
}

function d4os_io_services_groups_setAgentGroupSelectedRole($params, $inworld = TRUE) {
  global $requestingAgent, $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'setAgentGroupSelectedRole',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];
  $roleID = $params["RoleID"];

  if (isset($requestingAgent) && ($requestingAgent != UUID_ZERO) && ($requestingAgent != $agentID)) {
    return array(
      'error' => "Agent can only change their own Selected Group Role",
      'params' => var_export($params, TRUE)
    );
  }

  $return = _d4os_io_services_groups_setAgentGroupSelectedRole($params, $inworld);

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'setAgentGroupSelectedRole',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getAgentGroupMembership($params, $inworld = TRUE) {
  global $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getAgentGroupMembership',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

  // do the job
  $return = module_invoke_all('os_groups_services_getAgentGroupMembership', $params);

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getAgentGroupMembership',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getAgentGroupMemberships($params, $inworld = TRUE) {
  global $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getAgentGroupMemberships',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

  // do the job
  $return = module_invoke_all('os_groups_services_getAgentGroupMemberships', $params);

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getAgentGroupMemberships',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_canAgentViewRoleMembers($agentID, $groupID, $roleID) {
  global $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'canAgentViewRoleMembers',
    );
    $log[] = array(
      'name' => 'agentID',
      'type' => 'string',
      'data' => $agentID,
    );
    $log[] = array(
      'name' => 'groupID',
      'type' => 'string',
      'data' => $groupID,
    );
    $log[] = array(
      'name' => 'roleID',
      'type' => 'string',
      'data' => $roleID,
    );
    
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  $membersVisibleTo = variable_get('d4os_io_services_groups_membersVisibleTo', 'Group');
  
  if( $membersVisibleTo == 'All' ) return true;
  
  // do the job
  $viewMemberInfo = module_invoke_all('os_groups_services_canAgentViewRoleMembers', $params);
  if (isset($viewMemberInfo['error'])) {
    return $viewMemberInfo;
  }

  $return = FALSE;
  switch ($membersVisibleTo) {
    case 'Group':
      // if we get to here, there is at least one row, so they are a member of the group
      $return = TRUE;
    case 'Owners':
    default:
      $return = $viewMemberInfo['IsOwner'];			
  }

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'canAgentViewRoleMembers',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getGroupMembers($params, $inworld = TRUE) {
  global $requestingAgent, $groupPowers, $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getGroupMembers',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

  // do the job
  $members_list = module_invoke_all('os_groups_services_getGroupMembers', $params);
  if (isset($members_list['error'])) {
    return $members_list;
  }
  $members = $members_list['members'];
  $memberResults = $members_list['member_results'];

  if ($members == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No Group Members found',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }
  
  if (count($memberResults) == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No Visible Group Members found',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }
  
  $return = $memberResults;

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getGroupMembers',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getAgentActiveMembership($params, $inworld = TRUE) {
  global $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getAgentActiveMembership',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }
  
  d4os_io_services_groups_secureRequest($params, FALSE);

  // do the job
  $active_membership = module_invoke_all('os_groups_services_getAgentActiveMembership', $params);
  if (isset($active_membership['error'])) {
    return $active_membership;
  }
  $return = $active_membership;

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getAgentActiveMembership',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getAgentRoles($params, $inworld = TRUE) {
  global $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getAgentRoles',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

  // do the job
  $roles = module_invoke_all('os_groups_services_getAgentRoles', $params);
  if (isset($roles['error'])) {
    return $roles;
  }

  if (count($roles) == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'None found',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }

  $return = $roles;

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getAgentRoles',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getGroupRoles($params, $inworld = TRUE) {
  global $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getGroupRoles',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

  // do the job
  $roles = module_invoke_all('os_groups_services_getGroupRoles', $params);
  $roles = $roles[0];
  if (isset($roles['error'])) {
    return $roles;
  }

  if (count($roles) == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No roles found for group',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }
  
  $return = $roles;
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getGroupRoles',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getGroupRoleMembers($params, $inworld = TRUE) {
  global $requestingAgent, $groupPowers, $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getGroupRoleMembers',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

  // do the job
  $members_list = module_invoke_all('os_groups_services_getGroupRoleMembers', $params);
  if (isset($members_list['error'])) {
    return $$members_list;
  }
  $membersCount = $members_list['members_count'];
  $members = $members_list['members'];

  if ($membersCount == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No role memberships found for group',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }
  
  if (count($members) == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No rolememberships visible for group',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }

  $return = $members;
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getGroupRoleMembers',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_setAgentGroupInfo($params, $inworld = TRUE) {
  global $requestingAgent, $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'setAgentGroupInfo',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  // do the job
  $group_info = module_invoke_all('os_groups_services_setAgentGroupInfo', $params);
  if (isset($group_info['error'])) {
    return $group_info;
  }
  $return = $group_info;

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'setAgentGroupInfo',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getGroupNotices($params, $inworld = TRUE) {
  global $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getGroupNotices',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

  // do the job
  $notices = module_invoke_all('os_groups_services_getGroupNotices', $params);
  if (isset($notices['error'])) {
    return $notices;
  }

  if (count($notices) == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No Notices',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }
  
  $return = $notices;

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getGroupNotices',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getGroupNotice($params, $inworld = TRUE) {
  global $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getGroupNotice',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

  // do the job
  $notice = module_invoke_all('os_groups_services_getGroupNotice', $params);
  if (isset($notice['error'])) {
    return $notice;
  }
  
  if(!is_array($notice)) {
    return array(
      'succeed' => 'false',
      'error' => 'Group Notice Not Found',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }
  
  $return = $notice;

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getGroupNotice',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_addGroupNotice($params, $inworld = TRUE) {
  global $groupPowers, $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'addGroupNotice',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }
  
  if (is_array($error = d4os_io_services_groups_checkGroupPermission($params['GroupID'], $groupPowers['SendNotices']))) {
    return $error;
  }
  
  // do the job
  $notice = module_invoke_all('os_groups_services_addGroupNotice', $params);
  if (isset($notice['error'])) {
    return $notice;
  }
  $return = $notice;

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'addGroupNotice',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_addAgentToGroupInvite($params, $inworld = TRUE) {
  global $groupPowers, $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'addAgentToGroupInvite',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  if (is_array($error = d4os_io_services_groups_checkGroupPermission($params['GroupID'], $groupPowers['Invite']))) {
    return $error;
  }
  
  // do the job
  $agent = module_invoke_all('os_groups_services_addAgentToGroupInvite', $params);
  if (isset($agent['error'])) {
    return $agent;
  }
  $return = $agent;

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'addAgentToGroupInvite',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getAgentToGroupInvite($params, $inworld = TRUE) {
  global $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getAgentToGroupInvite',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }
  
  // do the job
  $agent = module_invoke_all('os_groups_services_getAgentToGroupInvite', $params);
  if (isset($agent['error'])) {
    return $agent;
  }
  $return = $agent;

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getAgentToGroupInvite',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_removeAgentToGroupInvite($params, $inworld = TRUE) {
  global $d4os_io_services_groups_log_level;
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'removeAgentToGroupInvite',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  // do the job
  $agent = module_invoke_all('os_groups_services_removeAgentToGroupInvite', $params);
  if (isset($agent['error'])) {
    return $agent;
  }
  $return = $agent;

  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'removeAgentToGroupInvite',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_secureRequest($params, $write = FALSE) {
  global $verifiedReadKey, $verifiedWriteKey, $requestingAgent;
  $overrideAgentUserService = variable_get('d4os_io_services_groups_overrideAgentUserService', '');
  $groupWriteKey = variable_get('d4os_io_services_groups_writeKey', '');
  $groupReadKey = variable_get('d4os_io_services_groups_readKey', '');
  $groupRequireAgentAuthForWrite = variable_get('d4os_io_services_groups_groupRequireAgentAuthForWrite', 0);
  // Cache this for access by other security functions
  $requestingAgent = $params['RequestingAgentID'];
  
  if (isset($groupReadKey) && ($groupReadKey != '') && (!isset($verifiedReadKey) || ($verifiedReadKey !== TRUE))) {
    if (!isset($params['ReadKey']) || ($params['ReadKey'] != $groupReadKey )) {
      return array(
        'error' => "Invalid (or No) Read Key Specified",
        'params' => var_export($params, TRUE)
      );
    }
    else {
      $verifiedReadKey = TRUE;
    }
  }
  
  if (($write == TRUE) && isset($groupWriteKey) && ($groupWriteKey != '') && (!isset($verifiedWriteKey) || ($verifiedWriteKey !== TRUE))) {
    if (!isset($params['WriteKey']) || ($params['WriteKey'] != $groupWriteKey)) {
      return array(
        'error' => "Invalid (or No) Write Key Specified",
        'params' => var_export($params, TRUE)
      );
    }
    else {
      $verifiedWriteKey = TRUE;
    }
  }
  
  if (($write == TRUE) && isset($groupRequireAgentAuthForWrite) && ($groupRequireAgentAuthForWrite == TRUE)) {
    // Note: my brain can't do boolean logic this morning, so just putting this here instead of integrating with line above.
    // If the write key has already been verified for this request, don't check it again.  This comes into play with methods that call other methods, such as CreateGroup() which calls Addrole()
    if (isset($verifiedWriteKey) && ($verifiedWriteKey !== TRUE)) {
      return TRUE;
    }
    
    if( !isset($params['RequestingAgentID']) 
    || !isset($params['RequestingAgentUserService'])
    || !isset($params['RequestingSessionID']) 
    ) {
      return array('error' => "Requesting AgentID and SessionID must be specified", 'params' => var_export($params, TRUE));
    }
    
    // NOTE: an AgentID and SessionID of UUID_ZERO will likely be a region making a request, that is not tied to a specific agent making the request.
    
    $UserService = $params['RequestingAgentUserService'];
    if (isset($overrideAgentUserService) && ($overrideAgentUserService != "")) {
      $UserService = $overrideAgentUserService;
    }
  
    $client = new xmlrpc_client($UserService);
    $client->return_type = 'phpvals';
  
    $verifyParams = new xmlrpcval(array(
      'avatar_uuid' => new xmlrpcval(
        $params['RequestingAgentID'],
        'string'
      ),
      'session_id' => new xmlrpcval(
        $params['RequestingSessionID'],
        'string')
      ),
      'struct'
    );
  
    $message = new xmlrpcmsg("check_auth_session", array($verifyParams));
    $resp = $client->send($message, 5);
    if ($resp->faultCode()) {
      return array(
        'error' => "Error validating AgentID and SessionID",
        'xmlrpcerror'=> $resp->faultString(),
        'params' => var_export($params, TRUE)
      );
    }
  
    $verifyReturn = $resp->value();
  
    if (!isset($verifyReturn['auth_session']) || ($verifyReturn['auth_session'] != 'TRUE')) {
      return array(
        'error' => "UserService.check_auth_session() did not return TRUE",
        'userservice' => var_export($verifyReturn, TRUE),
        'params' => var_export($params, TRUE)
      );
    }
  }
  
  return TRUE;
}

function d4os_io_services_groups_checkGroupPermission($GroupID, $Permission) {
  global $requestingAgent, $groupPowers, $d4os_io_services_groups_log_level;
  $groupEnforceGroupPerms = variable_get('d4os_io_services_groups_groupEnforceGroupPerms', 0);
  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'call function',
      'type' => 'string',
      'data' => 'checkGroupPermission',
    );
    $log[] = array(
      'name' => 'groupEnforceGroupPerms',
      'type' => 'var',
      'data' => $groupEnforceGroupPerms,
    );
    $log[] = array(
      'name' => 'groupID',
      'type' => 'var',
      'data' => $GroupID,
    );
    $log[] = array(
      'name' => 'Permission',
      'type' => 'var',
      'data' => $Permission,
    );
    d4os_log('d4os_io_srv_grps', $log, WATCHDOG_DEBUG);
  }

  if (!isset($Permission) || ($Permission == 0)) {
    return array(
      'error' => 'No Permission value specified for d4os_io_services_groups_checkGroupPermission',
      'Permission' => $Permission
    );
  }
  
  // If it isn't set to true, then always return true, otherwise verify they have perms
  if (!isset($groupEnforceGroupPerms) || ($groupEnforceGroupPerms != TRUE)) {
    return true;
  }
  
  if (!isset($requestingAgent) || ($requestingAgent == UUID_ZERO)) {
    return array(
      'error' => 'Requesting agent was either not specified or not validated.',
      'requestingAgent' => $requestingAgent
    );
  }
  
  $params = array('AgentID' => $requestingAgent, 'GroupID' => $GroupID);
  $reqAgentMembership = d4os_io_services_groups_getAgentGroupMembership($params, $inworld);
  
  if (isset($reqAgentMembership['error'])) {
    return array(
      'error' => 'Could not get agent membership for group',
      'params' => var_export($params, TRUE),
      'nestederror' => $reqAgentMembership['error']
    );
  }
  
  // Worlds ugliest bitwise operation, EVER
  $PermMask   = $reqAgentMembership['GroupPowers'];
  $PermValue  = $Permission;

  // do the job
  $PermMasked = module_invoke_all('os_groups_services_checkGroupPermission', $PermMask, $PermValue);
  $PermMasked = $PermMasked[0];
  
  if ($PermMasked != $Permission) {
    $permNames = array_flip($groupPowers);
    return array(
      'error' => 'Agent does not have group power to ' . $Permission .'('.$permNames[$Permission].')',
      'PermMasked' => $PermMasked,
      'params' => var_export($params, TRUE),
      'permBitMaskSql' => $sql,
      'Permission' => $Permission
    );
  }
  
  /*
  return array('error' => 'Reached end'
  , 'reqAgentMembership' => var_export($reqAgentMembership, TRUE)
  , 'GroupID' => $GroupID
  , 'Permission' => $Permission
  , 'PermMasked' => $PermMasked
  );
  */
  return TRUE;
}
