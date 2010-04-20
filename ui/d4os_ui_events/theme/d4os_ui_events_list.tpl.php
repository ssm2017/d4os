<?php foreach($events as $event): ?>
<div class="event node">
  <ul>
    <li><?php print $event->start_date; ?></li>
    <li><?php print $event->duration_string; ?></li>
    <li><?php print $event->url; ?></li>
  </ul>
</div>
<?php endforeach; ?>