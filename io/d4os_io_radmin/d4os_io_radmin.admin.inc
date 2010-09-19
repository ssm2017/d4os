<?php
/**
 * @package    d4os_io_radmin
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * Admin panel
 */
function d4os_io_radmin_admin() {
  $form = array();

  $form['d4os_io_radmin_password'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default radmin password'),
    '#default_value'  => variable_get('d4os_io_radmin_password', 'mypw'),
    '#description'    => t('Default radmin password'),
  );
  if (module_exists('d4os_osservices')) {
    $form['d4os_io_radmin_use_osservices'] = array(
      '#type'           => 'checkbox',
      '#title'          => t('Use osservices'),
      '#default_value'  => variable_get('d4os_io_radmin_use_osservices', 1),
      '#description'    => t('Use the osservices radmin proxy.'),
    );
  }
  return system_settings_form($form);
}
