<?php
/**
 * @package    d4os_io_services_offline_messages
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */
function d4os_io_services_offline_message_admin() {
  $form = array();

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
  $form['d4os_io_services_offline_message_log_level'] = array(
    '#type'           => 'select',
    '#title'          => t('Log level'),
    '#options'        => $options,
    '#default_value'  => variable_get('d4os_io_services_offline_message_log_level', 0),
    '#description'    => t('Define the log level'),
  );
  
  return system_settings_form($form);
}
