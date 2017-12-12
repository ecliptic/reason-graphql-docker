exports.up = function (knex, Promise) {
  return (
    // Kick off a promise chain
    Promise.resolve()

      // PaperClip
      .then(() =>
        knex.schema.createTable('paper_clips', function (table) {
          // entity data
          table
            .uuid('id')
            .primary()
            .notNullable()
            .unique()
            .defaultTo(knex.raw('uuid_generate_v4()'))
          table.timestamps()

          // fields
          table.string('size').notNullable()
        })
      )
  )
}

exports.down = function (knex, Promise) {
  throw new Error('Downward migrations are not supported. Restore from backup.')
}
