module.exports = {
  up: (queryInterface, Sequelize) => {
    return queryInterface.bulkInsert(
      'Tags',
      [
        {
          id: "3EDAE529",
          categoria_id: 1234671,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: "5249F21A",
          categoria_id: 1234672,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: "87CE683F",
          categoria_id: 1234673,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: "A9DEB9A2",
          categoria_id: 1234676,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: "F24A6920",
          categoria_id: 1234676,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: "F361 0621",
          categoria_id: 1234676,
          local: 4,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        }
      ],
      {}
    )
  },

  down: (queryInterface, Sequelize) => {
    return queryInterface.bulkDelete('Pessoas', null, {})
  }
}
