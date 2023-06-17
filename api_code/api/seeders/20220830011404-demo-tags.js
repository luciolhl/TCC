module.exports = {
  up: (queryInterface, Sequelize) => {
    return queryInterface.bulkInsert(
      'Tags',
      [
        {
          id: 1234671,
          categoria_id: 1,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 1234672,
          categoria_id: 3,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 1234673,
          categoria_id: 1,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 1234674,
          categoria_id: 2,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 1234675,
          categoria_id: 1,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 1234676,
          categoria_id: 2,
          local: 4,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 1234677,
          categoria_id: 3,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
      ],
      {}
    )
  },

  down: (queryInterface, Sequelize) => {
    return queryInterface.bulkDelete('Pessoas', null, {})
  }
}
