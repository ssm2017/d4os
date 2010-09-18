<?php
/**
 * @package    d4os_ui_grid_monitor
 * @subpackage hosts
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

function d4os_ui_grid_monitor_hosts_admin() {
  $form = array();

  $form['d4os_ui_grid_monitor_hosts_list'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Hosts list'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );

  $options = array(
    'SStats'      => 'SStats',
    'collector'   => 'Collector',
    'host'        => t('Host'),
    'port'        => t('Port'),
    'register'    => t('Register'),
    'lastcheck'   => t('LastCheck'),
    'failcounter' => t('FailCounter'),
    'status'      => t('Status'),
  );
  $form['d4os_ui_grid_monitor_hosts_list']['d4os_ui_grid_monitor_hosts_list_items'] = array(
    '#type'           => 'checkboxes',
    '#title'          => t('Items'),
    '#default_value'  => variable_get('d4os_ui_grid_monitor_hosts_list_items', array('host', 'port', 'register', 'lastcheck', 'failcounter', 'status')),
    '#options'        => $options,
    '#description'    => t('Select the items you want to display on the list.'),
  );

  return system_settings_form($form);
}
