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

function d4os_io_services_groups_admin() {
  $form = array();

  $form['keys'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Keys'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
    '#description'  => t("A xmlrpc client must have this key to commit changes to the Groups DB.Leave blank to allow all clients to make changes."),
  );
  $form['keys']['d4os_io_services_groups_writeKey'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Write key'),
    '#default_value'  => variable_get('d4os_io_services_groups_writeKey', ''),
  );
  $form['keys']['d4os_io_services_groups_readKey'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Read key'),
    '#default_value'  => variable_get('d4os_io_services_groups_readKey', ''),
  );
  $form['d4os_io_services_groups_groupRequireAgentAuthForWrite'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('groupRequireAgentAuthForWrite'),
    '#default_value'  => variable_get('d4os_io_services_groups_groupRequireAgentAuthForWrite', 0),
    '#description'    => t('Enabling this, will require that the service attempt to verify the agent is authentic by contacting the User Service specified in the request to authenticate the AgentID and SessionID provided.'),
  );
  $form['d4os_io_services_groups_groupEnforceGroupPerms'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('groupEnforceGroupPerms'),
    '#default_value'  => variable_get('d4os_io_services_groups_groupEnforceGroupPerms', 0),
    '#description'    => t('This enforces the role Permissions bitmask.'),
  );
  $form['d4os_io_services_groups_overrideAgentUserService'] = array(
    '#type'           => 'textfield',
    '#title'          => t('overrideAgentUserService'),
    '#default_value'  => variable_get('d4os_io_services_groups_overrideAgentUserService', ''),
    '#description'    => t('<p>Specify the following to hard-code / lockdown the User Service used to authenticate user sessions<br />
  Example: http://osgrid.org:8002<br />
  Note:  This causes the User Service specified with requests to be ignored, and prevents the service from being used cross-grid or by hypergridded users.</p>'),
  );
  $form['d4os_io_services_groups_membersVisibleTo'] = array(
    '#type'           => 'select',
    '#title'          => t('membersVisibleTo'),
    '#options'        => array('Group' => t('Group'), 'Owners' => t('Owners'), 'Roles' => t('Roles')),
    '#default_value'  => variable_get('d4os_io_services_groups_membersVisibleTo', 'Group'),
    '#description'    => t('Define who can see group members'),
  );
  $options = array(
    0 => t('Emergency'),
    1 => t('Alert'),
    2 => t('Critical'),
    3 => t('Error'),
    4 => t('Warning'),
    5 => t('Notice'),
    6 => t('Info'),
    7 => t('Debug'),
  );
  $form['d4os_io_services_groups_log_level'] = array(
    '#type'           => 'select',
    '#title'          => t('Log level'),
    '#options'        => $options,
    '#default_value'  => variable_get('d4os_io_services_groups_log_level', 0),
    '#description'    => t('Define the log level'),
  );
  
  return system_settings_form($form);
}
