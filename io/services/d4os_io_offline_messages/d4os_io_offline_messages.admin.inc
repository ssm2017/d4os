<?php

/**
 * @package    d4os_io_offline_messages
 * @copyright Copyright (C) 2010-2012 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @link      http://www.d4os.org
 * @license   GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * Admin panel
 */
function d4os_io_offline_messages_admin() {
  $form = array();

  $form['d4os_io_offline_messages_send_mails'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send mails'),
    '#default_value' => variable_get('d4os_io_offline_messages_send_mails', 0),
    '#description' => t('Select if you allow the users to receive emails for online messages.'),
  );
  // filters
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filters'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  $form['filters']['d4os_io_offline_messages_filter_allow_groups'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow group messages'),
    '#default_value' => variable_get('d4os_io_offline_messages_filter_allow_groups', 1),
    '#description' => t('Select if you allow the groups to store online messages.'),
  );
  $form['filters']['d4os_io_offline_messages_filter_dialogs'] = array(
    '#type' => 'textfield',
    '#title' => t('Filtered dialogs'),
    '#default_value' => variable_get('d4os_io_offline_messages_filter_dialogs', '42'),
    '#description' => t('Enter a comma separated list of ids to select filtered message dialogs. See !link for a complete list.', array('!link' => l('ImprovedInstantMessage', 'http://wiki.secondlife.com/wiki/ImprovedInstantMessage'))),
  );
  $form['filters']['d4os_io_offline_messages_filter_uuids'] = array(
    '#type' => 'textarea',
    '#title' => t('Forbidden uuids'),
    '#cols' => 37,
    '#default_value' => variable_get('d4os_io_offline_messages_filter_uuids', ''),
    '#description' => t('Enter the forbidden uuids, one by line.'),
  );
  // debug
  $form['debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('Debug'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
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
  $form['debug']['d4os_io_offline_messages_log_level'] = array(
    '#type' => 'select',
    '#title' => t('Log level'),
    '#options' => $options,
    '#default_value' => variable_get('d4os_io_offline_messages_log_level', 0),
    '#description' => t('Define the log level'),
  );

  return system_settings_form($form);
}
