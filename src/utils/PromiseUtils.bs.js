// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict'

function logMessage (exn) {
  var message = exn.message
  console.log(message !== undefined ? message : '(no error message)')
  return /* () */ 0
}

function cancelTimeout (delay, message) {
  return new Promise(function (_, reject) {
    setTimeout(function () {
      return reject(new Error(message))
    }, delay)
    return /* () */ 0
  })
}

exports.logMessage = logMessage
exports.cancelTimeout = cancelTimeout
/* No side effect */