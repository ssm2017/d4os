<?php

/**
 * @package    d4os_ui_auth
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
function d4os_ui_auth_settings() {
  $form = array();
  // get roles
  $roles = array(
    '0' => t('None'),
  );
  $roles += user_roles(TRUE);

  // login flow
  $options = array(
    0 => t('Drupal only'),
    1 => t('Grid only'),
    2 => t('Drupal -> Grid'),
    3 => t('Grid -> Drupal')
  );
  $form['login_flow'] = array(
    '#type' => 'fieldset',
    '#title' => t('Login flow'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['login_flow']['d4os_ui_auth_login_flow'] = array(
    '#type' => 'radios',
    '#title' => t('Login flow'),
    '#default_value' => variable_get('d4os_ui_auth_login_flow', 0),
    '#options' => $options,
    '#description' => t('Define the login flow.'),
  );
  $form['login_flow']['d4os_ui_auth_autocreate_grid_account'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autocreate grid account'),
    '#default_value' => variable_get('d4os_ui_auth_autocreate_grid_account', 0),
    '#description' => t('Auto create a grid account if the user is not registered on the grid and is allowed to create a grid user.'),
  );

  /*
   * Email value to set when creating a drupal user from the grid one
   */
  $form['users_synchro'] = array(
    '#type' => 'fieldset',
    '#title' => t('Users synchronization'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  // email generated
  $description = '<p>' . t('This email address is used when a user from the grid is created on the website and the user doesnt have an email address.') . '</p>'
      . '<p>' . t('You can use tokens from this list :') . '</p>'
      . '<ul>'
      . '<li>[uuid] ' . t('The uuid of the inworld user') . '</li>'
      . '<li>[livesite] ' . t('The url of the website') . '</li>'
      . '</ul>';
  $form['users_synchro']['d4os_ui_auth_default_missing_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Default email'),
    '#default_value' => variable_get('d4os_ui_auth_default_missing_email', '[uuid]@[livesite]'),
    '#description' => $description,
    '#required' => TRUE
  );
  // default role assigned to non email users
  $form['users_synchro']['d4os_ui_auth_default_missing_email_role'] = array(
    '#type' => 'select',
    '#title' => t('Assigned role to users without email'),
    '#default_value' => variable_get('d4os_ui_auth_default_missing_email_role', 0),
    '#options' => $roles,
    '#description' => t('This role is assigned to synchronized users without an email address.'),
  );
  return system_settings_form($form);
}
