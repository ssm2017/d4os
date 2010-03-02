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

/**
* Admin settings
*/
function d4os_ui_auth_settings() {
  $form = array();

  // login flow
  $options = array(
    0 => t('Drupal only'),
    1 => t('Grid only'),
    2 => t('Drupal -> Grid'),
    3 => t('Grid -> Drupal')
  );
  $form['d4os_ui_auth_login_flow'] = array(
    '#type'           => 'radios',
    '#title'          => t('Login flow'),
    '#default_value'  => variable_get('d4os_ui_auth_login_flow', 1),
    '#options'        => $options,
    '#description'    => t('Define the login flow.'),
  );

  return system_settings_form($form);
}