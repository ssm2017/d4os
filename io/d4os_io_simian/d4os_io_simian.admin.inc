<?php
/**
 * @package    d4os_io_simian
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
function d4os_io_simian_admin() {
  global $base_url;
  $form = array();

  $form['d4os_io_simian_url'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default simian url'),
    '#default_value'  => variable_get('d4os_io_simian_url', variable_get('d4os_default_grid_uri', $base_url). '/Grid/'),
    '#description'    => t("Default simian url (don't forget the trailing / )"),
  );
  return system_settings_form($form);
}

function d4os_io_simian_admin_validate($form, &$form_state) {
  // check the url
  $valid = FALSE;
  $http_answer = d4os_http_request($form_state['values']['d4os_io_simian_url'], array(), 'GET');
  if ($http_answer->code == 200) {
    if ($http_answer->data == 'SimianGrid') $valid = TRUE;
  }
  if (!$valid) {
    form_set_error('d4os_io_simian_url', t('There was an error when trying to contact the grid. Please check the url.'));
  }
}
