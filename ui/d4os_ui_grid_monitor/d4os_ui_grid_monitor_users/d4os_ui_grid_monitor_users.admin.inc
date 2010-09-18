<?php
/**
 * @package    d4os_ui_grid_monitor
 * @subpackage users
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

function d4os_ui_grid_monitor_users_admin() {
  $form = array();

  $form['d4os_ui_grid_monitor_users_list'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('users list'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );

  $options = array(
    'agentUuid'         => t('uuid'),
    'agentName'         => t('Name'),
    'agentIP'           => t('IP'),
    'agentPort'         => t('Port'),
    'loginTime'         => t('loginTime'),
    'logoutTime'        => t('logoutTime'),
    'currentRegion'     => t('currentRegion'),
    'currentRegionName' => t('currentRegionName'),
    'currentHandle'     => t('currentHandle'),
    'currentPos'        => t('currentPos'),
    'currentLookAt'     => t('currentLookAt'),
  );
  $form['d4os_ui_grid_monitor_users_list']['d4os_ui_grid_monitor_users_list_items'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Items'),
    '#default_value' => variable_get('d4os_ui_grid_monitor_users_list_items', array('agentUuid', 'agentName', 'agentIP', 'loginTime', 'currentRegionName')),
    '#options' => $options,
    '#description' => t('Select the items you want to display on the list.'),
  );

  return system_settings_form($form);
}
