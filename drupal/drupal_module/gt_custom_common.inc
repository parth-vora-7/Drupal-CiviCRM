<?php
/**
 * To get latest 3 case study nodes which are featured
 * @return array
 *     node ids
 */

function get_featured_case_study() {
  global $language;
  $case_nodes = db_select('node','n');
  $case_nodes->join('field_data_field_is_featured', 'fdfif', 'n.nid=fdfif.entity_id');
  $case_nodes->fields('n',array('nid'));
  $case_nodes->condition('fdfif.field_is_featured_value', 1);
  $case_nodes->condition('n.type', 'case_study');
  $case_nodes->condition('n.language', $language->language);
  $case_nodes->orderBy('nid','DESC');
  $case_nodes->range(0,3);
  $case_nodes = $case_nodes -> execute();
  $case_nodes = $case_nodes -> fetchCol('nid');
  return $case_nodes;
}

/**
 * To get latest 3 news/event nodes which are featured
 * @return array
 *     node ids
 */

function get_featured_news_event($limit) {
  global $language;
  $case_nodes = db_select('node','n');
  $case_nodes->join('field_data_field_is_featured', 'fdfif', 'n.nid=fdfif.entity_id');
  $case_nodes->join('field_data_field_archive', 'arc', 'n.nid=arc.entity_id');
  $case_nodes->fields('n',array('nid'));
  $case_nodes->condition('fdfif.field_is_featured_value', 1);
  $case_nodes->condition('arc.field_archive_value', 0);
  $or = db_or();
  $or->condition('n.type', 'news');
  $or->condition('n.type', 'event');
  $case_nodes->condition($or);
  $case_nodes->condition('n.language', $language->language);
  $case_nodes->orderBy('nid','DESC');
  $case_nodes->range(0,$limit);
  $case_nodes = $case_nodes -> execute();
  $case_nodes = $case_nodes -> fetchCol('nid');
  return $case_nodes;
}

/**
 * To get event name from event id
 * @param int $civi_event_id CiViCRm event id
 * @return string $civi_event_name event name 
 */

function get_civi_event_name($civi_event_id) {
  $civi_event_name = db_select('civi_events', 'ce')
    -> fields('ce', array('civi_event_name'))
    -> condition('civi_id', $civi_event_id)
    -> execute()
    -> fetchField();
  return $civi_event_name;
}
/**
 * To get home page slide nodes
 * @return array
 */

function get_slider_nodes() {
  global $language;
  $slider_nodes = db_select('node','n')
    -> fields('n',array('nid'))
    -> condition('n.type','home_page_slider')
    -> condition('n.status', 1)
    -> condition('n.language', $language->language)
    -> execute()
    -> fetchAll();
  return $slider_nodes;
}

