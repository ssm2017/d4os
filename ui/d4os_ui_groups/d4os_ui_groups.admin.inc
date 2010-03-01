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

function d4os_ui_groups_admin() {
	// check if og was configured
	$types = og_get_types('group');
	drupal_set_message('<pre>'.print_r($types, true).'</pre>', 'warning');
	drupal_set_message('<pre>'.print_r(og_types_map(), true).'</pre>', 'warning');
	$options = array();
	foreach ($types as $type) {
		$options[$type] = $type;
	}
	drupal_set_message('<pre>'.print_r($options, true).'</pre>', 'warning');
	$is_configured = count($types);
	if (!$is_configured) {
		drupal_set_message(t('Please choose a content type to use as a group.'), 'error');
		drupal_goto('admin/og/og');
	}
	$form = array();
	$form['d4os_ui_groups_type'] = array(
		'#type' 						=> 'select',
		'#title' 					=> t('Content type'),
		'#description' 		=> t('Select the content type to use as an inworld group.'),
		'#options' 				=> $options,
		'#default_value' 	=> variable_get('d4os_ui_groups_type', NULL),
	);
  return system_settings_form($form);
}
