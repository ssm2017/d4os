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