module.exports = {
  up: (queryInterface, Sequelize) => {
    return queryInterface.bulkInsert(
      'Tags',
      [
        {
          id: "3EDAE529",
          categoria_id: 1,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: "5249F21A",
          categoria_id: 2,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: "87CE683F",
          categoria_id: 3,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: "A9DEB9A2",
          categoria_id: 4,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: "F24A6920",
          categoria_id: 5,
          local: 2,
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: "F361 0621",
          categoria_id: 6,
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
