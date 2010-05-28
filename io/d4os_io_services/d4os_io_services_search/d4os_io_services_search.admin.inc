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

function d4os_io_services_search_admin() {
  $form = array();

  $form['d4os_io_services_search_parser_timer'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Timer'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_io_services_search_parser_timer', 15),
    '#description'    => t("Enter the value for the timer (in minutes) to refresh the data."),
  );

  $form['d4os_io_services_search_parser_host_qty'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Host parse qty'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_io_services_search_parser_host_qty', 1),
    '#description'    => t("Enter the quantity of hosts to parse on every cron run. (0 means no limit)"),
  );

  $form['d4os_io_services_search_parser_host_unreachable_limit'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Host unreachable limit'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_io_services_search_parser_host_unreachable_limit', 0),
    '#description'    => t("Limit before the host is deleted if unreachable. (for example, if set to 1, the host will be deleted if it can not been reach one time) (0 for no deletion)"),
  );

  $form['d4os_io_services_search_parser_bypass_expire'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Bypass sim expiration time'),
    '#default_value'  => variable_get('d4os_io_services_search_parser_bypass_expire', 0),
    '#description'    => t("Bypasses the sim recheck expiration time defined in OpenSim.ini."),
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
  $form['d4os_io_services_search_log_level'] = array(
    '#type'           => 'select',
    '#title'          => t('Log level'),
    '#options'        => $options,
    '#default_value'  => variable_get('d4os_io_services_search_log_level', 0),
    '#description'    => t('Define the log level'),
  );

  return system_settings_form($form);
}
