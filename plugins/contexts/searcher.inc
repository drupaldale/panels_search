<?php

$plugin = array(
  'title' => t("SearchAPI Searcher"),
  'description' => t('A SearchAPI Searcher, defaults to first active'),
  'context' => 'panels_search_context_create_searcher',
  'settings form' => 'panels_search_context_searcher_settings_form',
  'settings form validate' => 'panels_search_context_searcher_settings_form_validate',
  'settings form submit' => 'panels_search_context_searcher_settings_form_submit',
  'keyword' => 'searcher',
  'defaults' => array('type' => 'current'),
  'context name' => 'searcher',
);

/**
 * So right now we have the issue that contexts are all evaluated before the
 * panel panes are rendered. This means that a context that depends on a pane
 * being rendered will never show up.
 *
 * The fix for this is to alter how ctools/panels processes the contexts.
 *
 * So for this, by the time that a 'render last' pane is rendered, this context
 * should still be available. It would need to be re-processed then.
 *
 * So first, we need to keep the context around. We can achieve this by
 * 1) Also returning a context object
 * 2) Changing ctools to always keep context objects around and instead reprocess
 * each context before pane render. 
 *
 * Then we need a way for the context to re-process itself. That would be changing ctools
 * to call some method on the context. Any old-style context would just return the stored value.
 */
class context_searcher extends ctools_context {
}

function panels_search_context_create_searcher($empty, $data = NULL, $conf = FALSE) {
  $context = new context_searcher('string');
  $context->plugin = 'string';

  // Make sure to return a context obj for admin requests. 
  if ($empty || arg(0) == 'admin' || arg(0) == 'panels') {
    return $context;
  }

  $data = current(facetapi_get_active_searchers());

  if (!empty($data)) {
    $context->data     = $data;
    $context->title    = $data;
    return $context;
  }
}

function panels_search_context_searcher_settings_form($conf) {
  $form = array();
  return $form;
}

/**
 * Validate a searcher.
 */
function panels_search_context_searcher_settings_form_validate($form, &$form_values, &$form_state) {
}

function panels_search_context_searcher_settings_form_submit($form, &$form_values, &$form_state) {
}
