// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict'

var Curry = require('bs-platform/lib/js/curry.js')
var Utils = require('../utils/Utils.bs.js')
var Schema = require('../Schema.bs.js')
var PaperClipService = require('./PaperClipService.bs.js')

var resolvers = {
  PaperClip: {
    id: function (paperClip) {
      return Utils.getOrError(
        'A Paper Clip was returned with no id',
        paperClip.id
      )
    },
    createdAt: function (paperClip) {
      return Utils.getOrError(
        'A Link was returned with no created_at',
        paperClip.created_at
      )
    },
    updatedAt: function (paperClip) {
      return Utils.getOrError(
        'A Link was returned with no updated_at',
        paperClip.updated_at
      )
    },
    size: function (paperClip) {
      return paperClip.size
    },
  },
}

function make (dataProvider) {
  var service = PaperClipService.make(dataProvider)
  return /* record */ [
    /* resolvers */ resolvers,
    /* queries */ {
      allPaperClips: function (_, input, _$1) {
        var opt = input.filter
        if (opt == null) {
          return Curry._1(service[/* getAll */ 0], /* None */ 0)
        } else {
          return Curry._1(
            service[/* getAll */ 0],
            Schema.PaperClip[/* sizeFromJs */ 1](opt.size)
          )
        }
      },
      paperClip: function (_, input, _$1) {
        return Curry._1(service[/* getById */ 1], input.id)
      },
    },
    /* mutations */ {
      addPaperClip: function (_, input, _$1) {
        return Curry._1(service[/* add */ 2], input.paperClip)
      },
      updatePaperClip: function (_, input, _$1) {
        return Curry._2(service[/* update */ 3], input.id, input.paperClip)
      },
      removePaperClip: function (_, input, _$1) {
        return Curry._1(service[/* remove */ 4], input.id)
      },
    },
  ]
}

exports.resolvers = resolvers
exports.make = make
/* Utils Not a pure module */
