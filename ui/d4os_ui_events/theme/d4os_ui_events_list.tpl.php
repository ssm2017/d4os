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
<?php foreach($events as $event): ?>
<div class="event node">
  <ul>
    <li><?php print $event->start_date; ?></li>
    <li><?php print $event->duration_string; ?></li>
    <li><?php print $event->url; ?></li>
  </ul>
</div>
<?php endforeach; ?>
