module.exports = {
  up: (queryInterface, Sequelize) => {
    return queryInterface.bulkInsert(
      'categorias',
      [
        {
          id: 1234671,
          categorias: 1,
          item: "Toalha de rosto",
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 1234672,
          categorias: 1,
          item: "Toalha de corpo",
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 1234673,
          categorias: 1,
          item: "Roupao",
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 1234674,
          categorias: 2,
          item: "Lençol",
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 1234675,
          categorias: 2,
          item: "Fronha",
          deletedAt: null,
          createdAt: new Date(),
          updatedAt: new Date()
        },
        {
          id: 1234676,
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
