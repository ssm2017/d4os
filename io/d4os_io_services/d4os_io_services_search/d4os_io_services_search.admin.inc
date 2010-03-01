<?php
/**
* @package		d4os
* @copyright	Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
* @license		GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
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
  return system_settings_form($form);
}
