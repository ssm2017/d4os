<?php
/**
 * @package    d4os_io_services_groups
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

function d4os_io_services_groups_run($method, $params, $inworld=TRUE) {
  global $groupPowers, $d4os_io_services_groups_log_level;

  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => $method,
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_grps', $log, WATCHDOG_DEBUG);
  }

  switch ($method) {
    case 'createGroup':
    case 'addRoleToGroup':
    case 'updateGroupRole':
    case 'removeRoleFromGroup':
    case 'getGroup':
    case 'updateGroup':
    case 'findGroups':
    case 'setAgentActiveGroup':
    case 'addAgentToGroup':
    case 'removeAgentFromGroup':
    case 'addAgentToGroupRole':
    case 'removeAgentFromGroupRole':
    case 'setAgentGroupSelectedRole':
    case 'getAgentGroupMembership':
    case 'getAgentGroupMemberships':
    case 'getGroupMembers':
    case 'getAgentActiveMembership':
    case 'getAgentRoles':
    case 'getGroupRoles':
    case 'getGroupRoleMembers':
    case 'setAgentGroupInfo':
    case 'getGroupNotices':
    case 'getGroupNotice':
    case 'addGroupNotice':
    case 'addAgentToGroupInvite':
    case 'getAgentToGroupInvite':
    case 'removeAgentToGroupInvite':
       // check
      if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
        return $error;
      }
      break;
  }

  $check_group_perms = FALSE;
  switch ($method) {
    case 'addRoleToGroup':
      $check_group_perms = TRUE;
      $perm = $groupPowers['CreateRole'];
      break;
    case 'updateGroupRole':
    case 'removeRoleFromGroup':
      $check_group_perms = TRUE;
      $perm = $groupPowers['RoleProperties'];
      break;
    case 'updateGroup':
      $check_group_perms = TRUE;
      $perm = $groupPowers['ChangeOptions'];
      break;
    case 'removeAgentFromGroupRole':
      $check_group_perms = TRUE;
      $perm = $groupPowers['AssignMember'];
      break;
    case 'addGroupNotice':
      $check_group_perms = TRUE;
      $perm = $groupPowers['SendNotices'];
      break;
    case 'addAgentToGroupInvite':
      $check_group_perms = TRUE;
      $perm = $groupPowers['Invite'];
      break;
  }

  if ($check_group_perms) {
    if (is_array($error = d4os_io_services_groups_checkGroupPermission($params['GroupID'], $perm))) {
      return $error;
    }
  }

  switch ($method) {
    case 'createGroup':
    case 'updateGroupRole':
    case 'removeRoleFromGroup':
    case 'getGroup':
    case 'updateGroup':
    case 'findGroups':
    case 'removeAgentFromGroup':
    case 'addAgentToGroupRole':
    case 'removeAgentFromGroupRole':
    case 'getAgentGroupMembership':
    case 'getAgentGroupMemberships':
    case 'canAgentViewRoleMembers':
    case 'getGroupMembers':
    case 'getAgentActiveMembership':
    case 'getAgentRoles':
    case 'getGroupRoles':
    case 'getGroupRoleMembers':
    case 'setAgentGroupInfo':
    case 'getGroupNotices':
    case 'getGroupNotice':
    case 'addGroupNotice':
    case 'addAgentToGroupInvite':
    case 'getAgentToGroupInvite':
    case 'removeAgentToGroupInvite':
      $return = module_invoke_all('os_groups_services_'. $method, $params);
      break;
    case 'addRoleToGroup':
      $return = _d4os_io_services_groups_addRoleToGroup($params, $inworld);
      break;
    case 'setAgentActiveGroup':
      $return = _d4os_io_services_groups_setAgentActiveGroup($params, $inworld);
      break;
    case 'addAgentToGroup':
      $return = _d4os_io_services_groups_addAgentToGroup($params, $inworld);
      break;
    case 'setAgentGroupSelectedRole':
      $return = _d4os_io_services_groups_setAgentGroupSelectedRole($params, $inworld);
      break;
  }

  switch ($method) {
    case 'canAgentViewRoleMembers':
      $return = $return[0];
      break;
  }

  // log
  if ($d4os_io_services_groups_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => $method,
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_grps', $log, WATCHDOG_DEBUG);
  }

  return $return;
}
function d4os_io_services_groups_createGroup($params, $inworld = TRUE) {

  $return = d4os_io_services_groups_run('createGroup', $params, $inworld);

  // build group for og if group was created inworld
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
  return d4os_io_services_groups_run('createGroup', $params, $inworld);
}

function d4os_io_services_groups_updateGroupRole($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('updateGroupRole', $params, $inworld);
}

function d4os_io_services_groups_removeRoleFromGroup($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('removeRoleFromGroup', $params, $inworld);
}

function d4os_io_services_groups_getGroup($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('getGroup', $params, $inworld);
}

function _d4os_io_services_groups_getGroup($params, $inworld = TRUE) {
  // do the job
  return module_invoke_all('os_groups_services_getGroup', $params);
}        

function d4os_io_services_groups_updateGroup($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('updateGroup', $params, $inworld);
}

function d4os_io_services_groups_findGroups($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('findGroups', $params, $inworld);
}

function _d4os_io_services_groups_setAgentActiveGroup($params, $inworld = TRUE) {
  // do the job
  return module_invoke_all('os_groups_services_setAgentActiveGroup', $params);
}

function d4os_io_services_groups_setAgentActiveGroup($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('setAgentActiveGroup', $params, $inworld);
}

function d4os_io_services_groups_addAgentToGroup($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('addAgentToGroup', $params, $inworld);
}

// Private method, does not include security, to only be called from places that have already verified security
function _d4os_io_services_groups_addAgentToGroup($params, $inworld = TRUE) {
  // do the job
  return module_invoke_all('os_groups_services_addAgentToGroup_2', $params);
}

function d4os_io_services_groups_removeAgentFromGroup($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('removeAgentFromGroup', $params, $inworld);
}

function _d4os_io_services_groups_addAgentToGroupRole($params, $inworld = TRUE) {
  // do the job
  return module_invoke_all('os_groups_services_addAgentToGroupRole_2', $params);
}

function d4os_io_services_groups_addAgentToGroupRole($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('addAgentToGroupRole', $params, $inworld);
}

function d4os_io_services_groups_removeAgentFromGroupRole($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('removeAgentFromGroupRole', $params, $inworld);
}

function _d4os_io_services_groups_setAgentGroupSelectedRole($params, $inworld = TRUE) {
  // do the job
  return module_invoke_all('os_groups_services_setAgentGroupSelectedRole', $params);
}

function d4os_io_services_groups_setAgentGroupSelectedRole($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('setAgentGroupSelectedRole', $params, $inworld);
}

function d4os_io_services_groups_getAgentGroupMembership($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('getAgentGroupMembership', $params, $inworld);
}

function d4os_io_services_groups_getAgentGroupMemberships($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('getAgentGroupMemberships', $params, $inworld);
}

function d4os_io_services_groups_canAgentViewRoleMembers($params, $inworld=TRUE) {
  return d4os_io_services_groups_run('canAgentViewRoleMembers', $params, $inworld);
}

function d4os_io_services_groups_getGroupMembers($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('getGroupMembers', $params, $inworld);
}

function d4os_io_services_groups_getAgentActiveMembership($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('getAgentActiveMembership', $params, $inworld);
}

function d4os_io_services_groups_getAgentRoles($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('getAgentRoles', $params, $inworld);
}

function d4os_io_services_groups_getGroupRoles($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('getGroupRoles', $params, $inworld);
}

function d4os_io_services_groups_getGroupRoleMembers($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('getGroupRoleMembers', $params, $inworld);
}

function d4os_io_services_groups_setAgentGroupInfo($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('setAgentGroupInfo', $params, $inworld);
}

function d4os_io_services_groups_getGroupNotices($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('getGroupNotices', $params, $inworld);
}

function d4os_io_services_groups_getGroupNotice($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('getGroupNotice', $params, $inworld);
}

function d4os_io_services_groups_addGroupNotice($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('addGroupNotice', $params, $inworld);
}

function d4os_io_services_groups_addAgentToGroupInvite($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('addAgentToGroupInvite', $params, $inworld);
}

function d4os_io_services_groups_getAgentToGroupInvite($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('getAgentToGroupInvite', $params, $inworld);
}

function d4os_io_services_groups_removeAgentToGroupInvite($params, $inworld = TRUE) {
  return d4os_io_services_groups_run('removeAgentToGroupInvite', $params, $inworld);
}

function d4os_io_services_groups_secureRequest($params, $write = FALSE) {
  global $verifiedReadKey, $verifiedWriteKey, $requestingAgent;

  $overrideAgentUserService       = variable_get('d4os_io_services_groups_overrideAgentUserService', '');
  $groupWriteKey                  = variable_get('d4os_io_services_groups_writeKey', '');
  $groupReadKey                   = variable_get('d4os_io_services_groups_readKey', '');
  $groupRequireAgentAuthForWrite  = variable_get('d4os_io_services_groups_groupRequireAgentAuthForWrite', 0);

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
    d4os_log('d4os_io_svc_grps', $log, WATCHDOG_DEBUG);
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
