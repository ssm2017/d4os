<?php

/**
 * @package    d4os_io_groups
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
function d4os_io_groups_admin() {
  /*
   * Default group
   */
  $form['version'] = array(
    '#type' => 'fieldset',
    '#title' => t('Version'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['version']['d4os_io_groups_version'] = array(
    '#type' => 'radios',
    '#title' => t('Database group mechanism'),
    '#options' => array(0 => t('Flotsam'), 1 => t('V2')),
    '#default_value' => variable_get('d4os_io_groups_version', 0),
  );
  $form['default_group'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default group'),
    '#description' => t('Add new users to a group.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['default_group']['d4os_io_groups_default_group'] = array(
    '#type' => 'textfield',
    '#title' => t('Default group uuid'),
    '#size' => 36,
    '#maxlength' => 36,
    '#default_value' => variable_get('d4os_io_groups_default_group', UUID_ZERO),
  );

  return system_settings_form($form);
}

function d4os_io_groups_admin_validate($form, &$form_state) {
  return; // TODO : re-enable this (the problem is that the files are loaded at the module init so if we are using another group system, we can not load again the good file...)
  // check the default group
  $default_group_uuid = $form_state['values']['d4os_io_groups_default_group'];
  if ($default_group_uuid != UUID_ZERO && $default_group_uuid != '') {
    $d4os_group = D4OS_IO::create('Groups');
    $group_exists = $d4os_group->group_exists($default_group_uuid);
    if (!$group_exists) {
      form_set_error('d4os_io_groups_default_group', t('This uuid for group !uuid is not registered on the grid.', array('!uuid' => $default_group_uuid)));
      return;
    }
  }
}
