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

function d4os_ui_profile_admin() {
  $form = array();

  $form['d4os_ui_profile_enable_profile_tab'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable profile tab'),
    '#default_value' => variable_get('d4os_ui_profile_enable_profile_tab', 0),
    '#description' => t('Select if you want to enable the profile tab for users.'),
  );
  $form['d4os_ui_profile_disable_drupal_user_view_tab'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable Drupal user view tab'),
    '#default_value' => variable_get('d4os_ui_profile_disable_drupal_user_view_tab', 0),
    '#description' => t('Select if you want to disable the Drupal user view tab. You need to flush caches after changing this option.'),
  );
  $form['#submit'][] = 'd4os_ui_profile_admin_submit';

  return system_settings_form($form);
}

function d4os_ui_profile_admin_submit($form, &$form_state) {
  if ($form_state['values']['d4os_ui_profile_disable_drupal_user_view_tab'] != variable_get('d4os_ui_profile_disable_drupal_user_view_tab', 0)) {
    drupal_set_message(t('The cache menu was rebuilt.')); // del
    module_invoke('menu', 'rebuild');
  }
}