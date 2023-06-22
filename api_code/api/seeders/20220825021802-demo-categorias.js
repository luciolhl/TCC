module.exports = {
  up: (queryInterface, Sequelize) => {
    return queryInterface.bulkInsert(
      'Categorias',
      [
        {
          id: 1,
          categorias: 1,
          item: "Toalha de rosto",
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 2,
          categorias: 1,
          item: "Toalha de corpo",
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 3,
          categorias: 1,
          item: "Roupao",
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 4,
          categorias: 2,
          item: "Lençol",
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 5,
          categorias: 2,
          item: "Fronha",
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 6,
          categorias: 2,
          item: "Virol",
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
