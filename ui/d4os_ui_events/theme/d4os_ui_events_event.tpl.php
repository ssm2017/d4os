<?php
/**
 * @package    d4os_ui_events
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */
?>
<div class="event">
  <dl>
    <dt><?php print t('Title'); ?></dt>
      <dd><?php print $event->title; ?></dd>
    <dt><?php print t('Category'); ?></dt>
      <dd><?php print $event->category; ?></dd>
    <dt><?php print t('Mature'); ?></dt>
      <dd><?php print $event->mature; ?></dd>
    <dt><?php print t('Date'); ?></dt>
      <dd><?php print $event->date; ?></dd>
    <dt><?php print t('Duration'); ?></dt>
      <dd><?php print $event->duration_string; ?></dd>
    <dt><?php print t('Sim name'); ?></dt>
      <dd><?php print $event->simname; ?></dd>
    <dt><?php print t('Global pos'); ?></dt>
      <dd><?php print $event->globalPos; ?></dd>
    <dt><?php print t('Description'); ?></dt>
      <dd><?php print $event->body; ?></dd>
    <dt><?php print t('Cover charge'); ?></dt>
      <dd><?php print $event->covercharge; ?></dd>
  </dl>
</div>
