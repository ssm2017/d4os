<?php
/**
 * @package    d4os_ui_grid_monitor
 * @subpackage hosts
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */
?>
<div id="hostmanager-host">
  <div id="hostmanager-host-infos">
    <dl>
      <dt><?php print t('Expire'); ?></dt>
      <dd><?php print $host->expire; ?></dd>
      <dt><?php print t('Regions qty'); ?></dt>
      <dd><?php print count($host->region); ?></dd>
    </dl>
  </div><!-- hostmanager-host-infos -->
  <?php foreach ($host->region as $region): ?>
  <div class="hostmanager-region">
    <fieldset class="collapsible collapsed">
      <legend><?php print t('Region'); ?> : <?php print $region->info->name; ?></legend>
      <div class="fieldset-wrapper">
        <div class="hostmanager-region-info">
          <fieldset class="collapsible collapsed">
            <legend><?php print t('Info'); ?></legend>
            <div class="fieldset-wrapper">
              <dl>
                <dt><?php print t('Name'); ?></dt>
                <dd><?php print $region->info->name; ?></dd>
                <dt><?php print t('Category'); ?></dt>
                <dd><?php print $region['category']; ?></dd>
                <dt><?php print t('Entities'); ?></dt>
                <dd><?php print $region['entities']; ?></dd>
                <dt><?php print t('Uuid'); ?></dt>
                <dd><?php print $region->info->uuid; ?></dd>
                <dt><?php print t('Url'); ?></dt>
                <dd><?php print $region->info->url; ?></dd>
                <dt><?php print t('Handle'); ?></dt>
                <dd><?php print $region->info->handle; ?></dd>
                <dt><?php print t('Parcel qty'); ?></dt>
                <dd><?php print count($region->data->parceldata->parcel); ?></dd>
              </dl>
            </div><!-- fieldset-wrapper -->
          </fieldset>
        </div><!-- hostmanager-region-info -->
        <div class="hostmanager-region-estate">
          <div class="hostmanager-region-estate-user">
            <fieldset class="collapsible collapsed">
              <legend><?php print t('Estate user'); ?></legend>
              <div class="fieldset-wrapper">
                <dl>
                  <dt><?php print t('Name'); ?></dt>
                  <dd><?php print $region->data->estate->user->name; ?></dd>
                  <dt><?php print t('Uuid'); ?></dt>
                  <dd><?php print $region->data->estate->user->uuid; ?></dd>
                </dl>
              </div><!-- fieldset-wrapper -->
            </fieldset>
          </div><!-- hostmanager-region-estate-user -->
          <div class="hostmanager-region-estate-info">
            <fieldset class="collapsible collapsed">
              <legend><?php print t('Estate info'); ?></legend>
              <div class="fieldset-wrapper">
                <dl>
                  <dt><?php print t('Name'); ?></dt>
                  <dd><?php print $region->data->estate->name; ?></dd>
                  <dt><?php print t('ID'); ?></dt>
                  <dd><?php print $region->data->estate->id; ?></dd>
                  <dt><?php print t('Parent ID'); ?></dt>
                  <dd><?php print $region->data->estate->parentid; ?></dd>
                </dl>
              </div><!-- fieldset-wrapper -->
            </fieldset>
          </div><!-- hostmanager-region-estate-info -->
          <div class="hostmanager-region-estate-flags">
            <fieldset class="collapsible collapsed">
              <legend><?php print t('Estate flags'); ?></legend>
              <div class="fieldset-wrapper">
                <dl>
                  <dt><?php print t('Teleport'); ?></dt>
                  <dd><?php print $region->data->estate->flags['teleport']; ?></dd>
                  <dt><?php print t('Public'); ?></dt>
                  <dd><?php print $region->data->estate->flags['public']; ?></dd>
                </dl>
              </div><!-- fieldset-wrapper -->
            </fieldset>
          </div><!-- hostmanager-region-estate-flags -->
          <?php if (count($region->data->parceldata->parcel > 0)): ?>
          <div class="hostmanager-region-estate-parcels">
            <fieldset class="collapsible collapsed">
              <legend><?php print t('Parcels'); ?></legend>
              <div class="fieldset-wrapper">
              <?php foreach($region->data->parceldata->parcel as $parcel): ?>
                <div class="hostmanager-region-estate-parcel">
                  <fieldset class="collapsible collapsed">
                    <legend><?php print t('Parcel'); ?> : <?php print $parcel->name; ?></legend>
                    <div class="fieldset-wrapper">
                      <dl>
                        <dt><?php print t('Owner Uuid'); ?></dt>
                        <dd><?php print $parcel->owner->uuid; ?></dd>
                        <dt><?php print t('Owner'); ?></dt>
                        <dd><?php print $parcel->owner->name; ?></dd>
                        <dt><?php print t('Name'); ?></dt>
                        <dd><?php print $parcel->name; ?></dd>
                        <dt><?php print t('Description'); ?></dt>
                        <dd><?php print $parcel->description; ?></dd>
                        <dt><?php print t('Uuid'); ?></dt>
                        <dd><?php print $parcel->uuid; ?></dd>
                        <dt><?php print t('Area'); ?></dt>
                        <dd><?php print $parcel->area; ?></dd>
                        <dt><?php print t('Location'); ?></dt>
                        <dd><?php print $parcel->location; ?></dd>
                        <dt><?php print t('Info uuid'); ?></dt>
                        <dd><?php print $parcel->infouuid; ?></dd>
                        <dt><?php print t('Dwell'); ?></dt>
                        <dd><?php print $parcel->dwell; ?></dd>
                        <dt><?php print t('showinsearch'); ?></dt>
                        <dd><?php print $parcel['showinsearch']; ?></dd>
                        <dt><?php print t('scripts'); ?></dt>
                        <dd><?php print $parcel['scripts']; ?></dd>
                        <dt><?php print t('build'); ?></dt>
                        <dd><?php print $parcel['build']; ?></dd>
                        <dt><?php print t('public'); ?></dt>
                        <dd><?php print $parcel['public']; ?></dd>
                        <dt><?php print t('category'); ?></dt>
                        <dd><?php print $parcel['category']; ?></dd>
                        <dt><?php print t('forsale'); ?></dt>
                        <dd><?php print $parcel['forsale']; ?></dd>
                        <dt><?php print t('salesprice'); ?></dt>
                        <dd><?php print $parcel['salesprice']; ?></dd>
                      </dl>
                    </div><!-- fieldset-wrapper -->
                  </fieldset>
                </div><!-- hostmanager-region-estate-parcel -->
              <?php endforeach; // parcels ?>
            </div><!-- fieldset-wrapper -->
          </fieldset>
        </div>
          <?php endif; // parcels ?>
          <?php if (count($region->data->objectdata->object) > 0): ?>
          <div class="hostmanager-region-estate-objects">
            <fieldset class="collapsible collapsed">
              <legend><?php print t('Objects'); ?></legend>
              <div class="fieldset-wrapper">
              <?php foreach($region->data->objectdata->object as $object): ?>
                <div class="hostmanager-region-estate-object">
                  <fieldset class="collapsible collapsed">
                    <legend><?php print t('object'); ?> : <?php print $object->title; ?></legend>
                    <div class="fieldset-wrapper">
                      <dl>
                        <dt><?php print t('Uuid'); ?></dt>
                        <dd><?php print $object->uuid; ?></dd>
                        <dt><?php print t('Title'); ?></dt>
                        <dd><?php print $object->title; ?></dd>
                        <dt><?php print t('Description'); ?></dt>
                        <dd><?php print $object->description; ?></dd>
                        <dt><?php print t('Description'); ?></dt>
                        <dd><?php print $object->description; ?></dd>
                        <dt><?php print t('Flags'); ?></dt>
                        <dd><?php print $object->flags; ?></dd>
                        <dt><?php print t('regionuuid'); ?></dt>
                        <dd><?php print $object->regionuuid; ?></dd>
                        <dt><?php print t('parceluuid'); ?></dt>
                        <dd><?php print $object->parceluuid; ?></dd>
                      </dl>
                    </div><!-- fieldset-wrapper -->
                  </fieldset>
                </div><!-- hostmanager-region-estate-object -->
              <?php endforeach; // objects ?>
              </div><!-- fieldset-wrapper -->
            </fieldset>
          </div>
          <?php endif; // objects ?>
        </div><!-- hostmanager-region-estate -->
      </div><!-- fieldset-wrapper -->
    </fieldset>
  </div><!-- hostmanager-region -->
  <?php endforeach; // regions ?>
</div><!-- hostmanager-host -->
