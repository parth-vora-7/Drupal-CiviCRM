<?php

module_load_include('inc', 'gt_custom', '/gt_custom_common');

/**
 * Implements hook_query_alter().
 * @param array $$view
 *   view array
 * @param array $query
 *   view query
 * @return array
 * 	 returns custom blocks
 */

function gt_custom_views_query_alter(&$view, $query) {

   if($view->name == 'case_studies' && $view->current_display == 'block') {

    /* if no any case study marked as featured then show recent 3 case studies */
    $total_featured_cases = count(get_featured_case_study()); 
    if($total_featured_cases == 3) {
        $view->set_items_per_page(-1);
    } else if($total_featured_cases == 2) {
        $view->set_items_per_page(1);
    } else if($total_featured_cases == 1) {
        $view->set_items_per_page(2);
    } else if($total_featured_cases == 0) {
        $view->set_items_per_page(3);
    }
  } else if($view->name == 'news_and_events' && $view->current_display == 'block') {
    /* if no any case study marked as featured then show recent 3 case studies */
    $total_featured_cases = count(get_featured_news_event(3)); 

    if($total_featured_cases == 3) {
        $view->set_items_per_page(-1);
    } else if($total_featured_cases == 2) {
        $view->set_items_per_page(1);
    } else if($total_featured_cases == 1) {
        $view->set_items_per_page(2);
    } else if($total_featured_cases == 0) {
        $view->set_items_per_page(3);
    }
  } else if($view->name == 'news_and_events' && $view->current_display == 'block_1') {
    /* if no any case study marked as featured then show recent 3 case studies */
    $total_featured_cases = count(get_featured_news_event(4)); 
    
    if($total_featured_cases == 4) {
        $view->set_items_per_page(-1);
    } else if($total_featured_cases == 3) {
        $view->set_items_per_page(1);
    } else if($total_featured_cases == 2) {
        $view->set_items_per_page(2);
    } else if($total_featured_cases == 1) {
        $view->set_items_per_page(3);
    } else if($total_featured_cases == 0) {
        $view->set_items_per_page(4);
    }
  } else if($view->name == 'related_articles' && $view->current_display == 'block') {
    /* if no any case study marked as featured then show recent 3 case studies */
    $arg = arg();
    if (isset($arg[1])):
        $node = node_load($arg[1]);
        $type = $node->type;
        if ($type == 'page' || $type == 'news' || $type == 'event'):
                $join = new views_join;
                $join->table ='field_data_field_archive';
                $join->left_table = 'node';
                $join->left_field = 'nid';
                $join->field = 'entity_id';
                $join->type = "LEFT";
                $join->extra_type = 'AND';
                $join->adjusted = 'TRUE';
                $view->query->table_queue['field_archive'] = array(
                                                            'table' => 'field_data_field_archive',
                                                            'num' => 1,
                                                            'alias' => 'field_data_field_archive',
                                                            'join' => $join,
                                                            'relationship' => 'node'
                                                          );
                 $view->query->tables['node']['field_archive'] = array(
                                                            'count' => 1,
                                                            'alias' => 'field_data_field_archive'
                                                          );
            
            $view->query->where[1]['conditions'][] = array('field' => 'node.type', 'value' => array('news','event'), 'operator' => 'in');
            $query->where[1]['conditions'][] = array('field' => 'field_data_field_archive.field_archive_value','value' => 0, 'operator' => '=');     
            $view->set_items_per_page(4);
            if ($type == 'page'):
                $view->set_items_per_page(3);
            endif;
        elseif($type == 'case_study'):
          $view->query->where[1]['conditions'][] = array('field' => 'node.type', 'value' => array('case_study'), 'operator' => 'in');
          $view->set_items_per_page(4);
        elseif($type == 'campaigns'):
          $view->query->where[1]['conditions'][] = array('field' => 'node.type', 'value' => array('campaigns'), 'operator' => 'in');
          $view->set_items_per_page(4);
        endif;
    endif;
  }     
}

/**
 * Implements hook_views_pre_render().
 * @param array $view
 *   view array
 * @return array
 * 	 returns view output
 */

function gt_custom_views_pre_render(&$view) {
  if($view->name == 'case_studies' && $view->current_display == 'block') {
    $featured_cases = get_featured_case_study(); 
    if (count($featured_cases) == 3):
      $view->result = array();
    endif;
    foreach ($featured_cases as $key=>$value) {
      
      $view_result = count($view->result);
      $case_node = node_load($value);
      $object = new stdClass();
      $object->nid = $case_node->nid;
      $object->node_title = $case_node->title;
      $object->node_language= $case_node->language;
      $object->node_created= $case_node->created;
      $object->field_data_field_image_node_entity_type= 'node';
      $object->field_data_field_teaser_text_node_entity_type = 'node';

      if(isset($case_node->field_image['und'][0])) {
        $object->field_field_image[0]['rendered'] = array(
          '#theme'=>'image_formatter',
          '#item'=>$case_node->field_image['und'][0],
          '#image_style' => 'feature_image',
          '#path'=>'',
          '#access'=> 1
        );    
      } else {
        $object->field_field_image[0]['rendered'] = '';
      }

      if(isset($case_node->field_teaser_text['und'][0]['value'])) {
        $object->field_field_teaser_text[0]['rendered'] = array(
          '#markup'=> $case_node->field_teaser_text['und'][0]['value'],
          '#access'=> 1
        );            
      }
      $view->result[$view_result + 1] = $object;
    }
    
  }
  else if($view->name == 'news_and_events' && $view->current_display == 'block') {
    $featured_news_event = get_featured_news_event(3);
    if (count($featured_news_event) == 3):
      $view->result = array();
    endif;
    foreach ($featured_news_event as $key=>$value) {
      $view_result = count($view->result);
      $news_event_node = node_load($value);
        $object = new stdClass();
        $object->nid = $news_event_node->nid;
        $object->node_title = $news_event_node->title;
        $object->node_language= $news_event_node->language;
        $object->node_created= $news_event_node->created;
        $object->field_data_field_image_node_entity_type= 'node';
        $object->field_data_field_teaser_text_node_entity_type = 'node';

        if(isset($news_event_node->field_image['und'][0])) {
          $object->field_field_image[0]['rendered'] = array(
            '#theme'=>'image_formatter',
            '#item'=>$news_event_node->field_image['und'][0],
            '#image_style' => 'feature_image',
            '#path'=>'',
            '#access'=> 1
          );    
        } else {
          $object->field_field_image[0]['rendered'] = '';
        }

        if(isset($news_event_node->field_teaser_text['und'][0]['value'])) {
          $object->field_field_teaser_text[0]['rendered'] = array(
            '#markup'=> $news_event_node->field_teaser_text['und'][0]['value'],
            '#access'=> 1
          );            
        } else {
          $object->field_field_teaser_text[0]['rendered'] = '';
        }
        $view->result[$view_result + 1] = $object;
    }
  }
  else if($view->name == 'news_and_events' && $view->current_display == 'block_1') {
    $featured_news_event = get_featured_news_event(4);
    if (count($featured_news_event) == 4):
      $view->result = array();
    endif;
    foreach ($featured_news_event as $key=>$value) {
      $view_result = count($view->result);
      $news_event_node = node_load($value);
        $object = new stdClass();
        $object->nid = $news_event_node->nid;
        $object->node_title = $news_event_node->title;
        $object->node_language= $news_event_node->language;
        $object->node_created= $news_event_node->created;
        $object->field_data_field_image_node_entity_type= 'node';
        $object->field_data_field_teaser_text_node_entity_type = 'node';

        if(isset($news_event_node->field_image['und'][0])) {
          $object->field_field_image[0]['rendered'] = array(
            '#theme'=>'image_formatter',
            '#item'=>$news_event_node->field_image['und'][0],
            '#image_style' => 'feature_image',
            '#path'=>'',
            '#access'=> 1
          );    
        } else {
          $object->field_field_image[0]['rendered'] = '';
        }

        if(isset($news_event_node->field_teaser_text['und'][0]['value'])) {
          $object->field_field_teaser_text[0]['rendered'] = array(
            '#markup'=> $news_event_node->field_teaser_text['und'][0]['value'],
            '#access'=> 1
          );            
        } else {
          $object->field_field_teaser_text[0]['rendered'] = '';
        }
        $view->result[$view_result + 1] = $object;
    }
  }
}

function gt_custom_submenu_tree_all_data($title, $menu = 'primary-links') {
  $tree = menu_tree_all_data($menu);
  foreach ($tree as $branch) {
    if ($branch['link']['title'] == $title) {
      return $branch['below'];
    }
  }
  return array();
}
